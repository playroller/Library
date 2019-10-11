#include <vector>   

template <class T> class BinaryIndexedTree {
  private:
    int n;
    std::vector<T> bit;

  public:
    BinaryIndexedTree(int node) : n(node), bit(node + 1, 0) {}

    inline T sum(int i) {
      ++i;
      T s = 0;
      while(i > 0) {
        s += bit[i];
        i -= i & -i;
      }
      return s;
    }

    inline T sum(int a, int b) {
      return sum(b) - sum(a - 1);
    }

    inline void add(int i, T x) {
      ++i;
      while(i <= n) {
        bit[i] += x;
        i += i & -i;
      }
    }

    int get(long long k) {
      ++k;
      int res = 0, N = 1;
      while(N < bit.size()) N *= 2;
      for(int i = N / 2; i > 0; i /= 2) {
        if(res + i < bit.size() && bit[res + i] < k) {
          k -= bit[res + i];
          res += i;
        }
      }
      return res + 1;
    }
};