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
      weak_node parent;
      node_ptr right_shared;
      weak_node right;
      weak_node left;
      node_ptr child;
      bool mark = false;
    };

    node_ptr min = NULL;
    int sz = 0;

    void addRoot(node_ptr x) {
      x->mark = false;

      if(min == NULL) {
        min = x;
        min->right = x;
        min->left = x;
        return;
      }

      node_ptr l = min->left.lock();
      l->right_shared = x;
      l->right = x;
      min->left = x;

      x->right = min;
      x->left = l;
    }

    void removeRoot(node_ptr x) {
      if(x->right_shared) {
        node_ptr r = x->right_shared;
        node_ptr l = x->left.lock();

        r->left = l;
        l->right_shared = r;
        l->right = r;

        x->right_shared.reset();
      }
      else if(!x->right_shared) {
        node_ptr r = x->right.lock();
        node_ptr l = x->left.lock();

        r->left = l;
        l->right = r;
        l->right_shared.reset();
      }
    }

    void addChild(node_ptr child, node_ptr parent) {
      removeRoot(child);

      if(parent->degree == 0) {
        parent->child = child;
        child->left = child;
        child->right = child;
      }
      else {
        node_ptr pchild = parent->child;
        node_ptr l = pchild->left.lock();
        l->right_shared = child;
        l->right = child;
        pchild->left = child;

        child->right = pchild;
        child->left = l;
      }
      parent->degree++;
      child->parent = parent;
    }

    void removeChild(node_ptr child, node_ptr parent) {
      if(parent->child == child) {
        if(parent->degree == 1) {
          parent->child.reset();
        }
        else {
          node_ptr r = child->right_shared;
          node_ptr l = child->left.lock();
          parent->child = r;
          r->left = l;
          l->right = r;
          child->right_shared.reset();
        }
      }
      else if(parent->child == child->right.lock()) {
        node_ptr l = child->left.lock();
        l->right = parent->child;
        l->right_shared.reset();
        parent->child->left = l;
      }
      else {
        node_ptr l = child->left.lock();
        l->right = child->right_shared;
        l->right_shared = child->right_shared;
        child->right_shared->left = l;
        child->right_shared.reset();
      }
      parent->degree--;
      child->parent.reset();
    }

    void minUpdate(node_ptr x) {
      node_ptr l = min->left.lock();
      l->right_shared = min;
      l->right = min;

      l = x->left.lock();
      l->right = x;
      l->right_shared.reset();
      min = x;
    }

    void consolidate() {
      std::vector<node_ptr> rootList;
      rootList.emplace_back(min);

      while(rootList[rootList.size() - 1]->right.lock() != rootList[0]) {
        node_ptr lastRight = rootList[rootList.size() - 1]->right_shared;
        rootList.emplace_back(lastRight);
      }

      int log2 = 0;
      {
        int n = 1;
        while(sz >= n) {
          log2++;
          n *= 2;
        }
      }

      std::vector<node_ptr> rootOfDegree(log2 + 1, NULL);
      for(auto &parent : rootList) {
        int d = parent->degree;
        while(rootOfDegree[d] != NULL) {
          node_ptr child = rootOfDegree[d];
          if(parent->key > child->key) std::swap(child, parent);
          addChild(child, parent);
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
          node->right_shared.reset();
          min = node;
        }
        else {
          addRoot(node);
          if(node->key < min->key) minUpdate(node);
        }
      }
    }
    
    bool isRoot(node_ptr x) {
      return x->parent.expired();
    }

  public:
    struct pointer {
      weak_node node;

      bool expired() {
        return node.expired();
      }
    };

    bool empty() {
      return (sz == 0);
    }

    int size() {
      return sz;
    }

    T top() {
      return min->key;
    }

    void pop() {
      node_ptr z = min;

      node_ptr child = z->child;
      for(int i = 0; i < z->degree; ++i) {
        addRoot(child);
        child->parent.reset();
        child = child->right_shared;
      }

      removeRoot(z);
      if(sz == 1) min = NULL;
      else {
        min = z->right.lock();
        consolidate();
      }
      sz--;
    }

    pointer push(T key) {
      node_ptr x = std::make_shared<Node>();
      x->key = key;
      
      addRoot(x);
      if(x->key < min->key) minUpdate(x);
      sz++;

      pointer ptr;
      ptr.node = x;
      return ptr;
    }

    void decrease(T key, pointer ptr) {
      node_ptr x = ptr.node.lock();
      x->key = key;

      if(!isRoot(x) && x->key < x->parent.lock()->key) {
        node_ptr parent = x->parent.lock();
        removeChild(x, parent);
        addRoot(x);
        
        node_ptr child = parent;
        while(child->mark) {
          parent = child->parent.lock();
          removeChild(child, parent);
          addRoot(child);
          child = parent;
        }
        if(!isRoot(child)) parent->mark = true;
      }

      if(x->key < min->key) minUpdate(x);
    }

    static FibonacciHeap merge(FibonacciHeap &h1, FibonacciHeap &h2) {
      // TODO
    }
};