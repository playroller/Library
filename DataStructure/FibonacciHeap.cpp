#include <iostream>
#include <vector>
#include <memory>

template <class T> class FibonacciHeap {
  private:
    struct Node;
    using node_ptr = std::shared_ptr<Node>;
    using weak_node = std::weak_ptr<Node>;

    struct Node {
      T key;
      int degree = 0;
      weak_node p;
      weak_node right;
      weak_node left;
      std::vector<node_ptr> children;
      node_ptr own = NULL;

      Node() {}
      Node(T key, int degree, weak_node p, weak_node right, weak_node left, std::vector<node_ptr> children, node_ptr own)
      : key(key), degree(degree), p(p), right(right), left(left), children(children), own(own) {}
    };

    node_ptr min = NULL;
    int sz = 0;

    void addRoot(node_ptr x) {
      if(min == NULL) {
        min = x;
        min->right = x;
        min->left = x;
        return;
      }

      node_ptr l = min->left.lock();
      l->right = x;
      min->left = x;

      x->right = min;
      x->left = l;
    }

    void removeRoot(node_ptr x) {
      x->own = NULL;
      node_ptr r = x->right.lock();
      node_ptr l = x->left.lock();
      r->left = l;
      l->right = r;
    }

    void link(node_ptr child, node_ptr parent) {
      removeRoot(child);
      parent->degree++;
      parent->children.emplace_back(child);
      child->p = parent;
    }

    void consolidate() {
      std::vector<node_ptr> rootList;
      rootList.emplace_back(min);
      while(rootList[rootList.size() - 1]->right.lock() != rootList[0]) {
        node_ptr lastRight = rootList[rootList.size() - 1]->right.lock();
        rootList.emplace_back(lastRight);
      }

      std::vector<node_ptr> rootOfDegree(sz + 1, NULL);
      for(auto &parent : rootList) {
        int d = parent->degree;
        while(rootOfDegree[d] != NULL) {
          node_ptr child = rootOfDegree[d];
          if(parent->key > child->key) std::swap(child, parent);
          link(child, parent);
          rootOfDegree[d] = NULL;
          d++;
        }
        rootOfDegree[d] = parent;
      }

      min = NULL;
      for(auto &node : rootOfDegree) {
        if(node == NULL) continue;

        node->own = node;
        if(min == NULL) {
          node->right = node;
          node->left = node;
          min = node;
        }
        else {
          addRoot(node);
          if(node->key < min->key) min = node;
        }
      }
    }

    void insert(node_ptr x) {
      if(min == NULL) {
        min = x;
        x->right = x;
        x->left = x;
      }
      else {
        addRoot(x);
        if(x->key < min->key) min = x;
      }
      sz++;
    }

  public:
    ~FibonacciHeap() {
      if(min == NULL) return;

      node_ptr node = min;
      while(node->right.lock() != NULL) {
        node = node->right.lock();
        node->left.lock()->own = NULL;
      }
    }

    bool empty() {
      return (sz == 0);
    }

    int size() {
      return sz;
    }

    T pop() {
      node_ptr z = min;
      T res = z->key;

      for(auto &child : z->children) {
        addRoot(child);
        child->p.lock() = NULL;
      }
      removeRoot(z);
      if(z == z->right.lock()) min = NULL;
      else {
        min = z->right.lock();
        consolidate();
      }
      sz--;
      return res;
    }

    void insert(T key) {
      node_ptr x = std::make_shared<Node>();
      x->key = key;
      x->own = x;
      insert(x);
    }

    static FibonacciHeap merge(FibonacciHeap &h1, FibonacciHeap &h2) {
      FibonacciHeap h;
      h.min = h1.min;
      if((h1.min == NULL) || (h2.min != NULL && h2.min->key < h1.min->key)) h.min = h2.min;

      if(h1.min != NULL && h2.min != NULL) {
        node_ptr n1 = h1.min;
        node_ptr n2 = h2.min;
        node_ptr n1l = n1->left.lock();
        node_ptr n2r = n2->right.lock();

        n1->left = n2;
        n2->right = n1;
        n2r->left = n1l;
        n1l->right = n2r;
      }

      h.sz = h1.sz + h2.sz;
      return h;
    }
};