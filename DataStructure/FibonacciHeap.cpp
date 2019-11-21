#include <iostream>
#include <vector>
#include <memory>

template <class T> class FibonacciHeap {
  struct Node {
    T key;
    int degree = 0;
    Node *p = NULL;
    Node *right = NULL;
    Node *left = NULL;
    std::vector<Node*> children;
    bool mark = false;

    Node() {}
    Node(T key, int degree, Node *p, Node *right, Node *left, std::vector<Node*> children, bool mark)
     : key(key), degree(degree), p(p), right(right), left(left), children(children), mark(mark) {}
  };

  private:
    Node *min = NULL;
    int sz = 0;

    using node_ptr = std::shared_ptr<Node>;
    std::vector<node_ptr> sp;

    void addRoot(Node *x) {
      if(min == NULL) {
        min = x;
        min->right = x;
        min->left = x;
        return;
      }

      Node *l = min->left;
      l->right = x;
      min->left = x;

      x->right = min;
      x->left = l;
    }

    void removeRoot(Node *x) {
      Node *r = x->right;
      Node *l = x->left;
      r->left = l;
      l->right = r;
    }

    void link(Node *child, Node *parent) {
      removeRoot(child);
      parent->degree++;
      parent->children.emplace_back(child);
      child->p = parent;
      child->mark = false;
    }

    void consolidate() {
      std::vector<Node*> rootList;
      rootList.emplace_back(min);
      while(rootList[rootList.size() - 1]->right != rootList[0]) {
        Node *lastRight = rootList[rootList.size() - 1]->right;
        rootList.emplace_back(lastRight);
      }

      std::vector<Node*> rootOfDegree(sz + 1, NULL);
      for(auto &parent : rootList) {
        int d = parent->degree;
        while(rootOfDegree[d] != NULL) {
          Node *child = rootOfDegree[d];
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

    void insert(Node *x) {
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
    bool empty() {
      return (sz == 0);
    }

    int size() {
      return sz;
    }

    T pop() {
      Node *z = min;
      T res = z->key;

      for(auto &child : z->children) {
        addRoot(child);
        child->p = NULL;
      }
      removeRoot(z);
      if(z == z->right) min = NULL;
      else {
        min = z->right;
        consolidate();
      }
      sz--;
      return res;
    }

    void insert(T key) {
      Node *x = new Node;
      sp.push_back(node_ptr(x));
      x->key = key;
      insert(x);
    }

    static FibonacciHeap merge(FibonacciHeap &h1, FibonacciHeap &h2) {
      FibonacciHeap h;
      h.min = h1.min;
      if((h1.min == NULL) || (h2.min != NULL && h2.min->key < h1.min->key)) h.min = h2.min;

      if(h1.min != NULL && h2.min != NULL) {
        Node *n1 = h1.min;
        Node *n2 = h2.min;
        Node *n1l = n1->left;
        Node *n2r = n2->right;

        n1->left = n2;
        n2->right = n1;
        n2r->left = n1l;
        n1l->right = n2r;
      }

      h.sz = h1.sz + h2.sz;
      return h;
    }
};