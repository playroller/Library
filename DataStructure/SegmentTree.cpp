#include <vector>
#include <functional>

template <class Monoid> class SegmentTree {
  private:
    using F = std::function<Monoid(Monoid, Monoid)>;

    int sz;
    std::vector<Monoid> seg;

    const F f;
    const Monoid M1;

  public:
    SegmentTree(int n, const F f, const Monoid &M1) : f(f), M1(M1) {
      sz = 1;
      while(sz < n) sz *= 2;
      seg.assign(2 * sz, M1);
    }

    void update(int k, const Monoid &x) {
      k += sz - 1;
      seg[k] = x;
      while(k > 0) {
        k = (k - 1) / 2;
        seg[k] = f(seg[2 * k + 1], seg[2 * k + 2]);
      }
    }

    void add(int k, const Monoid &x) {
      k += sz - 1;
      seg[k] += x;
      while(k > 0) {
        k = (k - 1) / 2;
        seg[k] = f(seg[2 * k + 1], seg[2 * k + 2]);
      }
    }

    Monoid query(int a, int b, int k = 0, int l = 0, int r = -1) {
      if(r == -1) r = sz;

      if(b <= l || r <= a) return M1;
      if(a <= l && r <= b) return seg[k];
      Monoid vl = query(a, b, 2 * k + 1, l, (l + r) / 2);
      Monoid vr = query(a, b, 2 * k + 2, (l + r) / 2, r);
      return f(vl, vr);
    }
};