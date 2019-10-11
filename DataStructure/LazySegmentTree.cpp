#include <vector>
#include <functional>

template <class Monoid, class OperatorMonoid = Monoid> class LazySegmentTree {
  using F = std::function<Monoid(Monoid, Monoid)>;
  using G = std::function<Monoid(Monoid, OperatorMonoid, int)>;
  using H = std::function<OperatorMonoid(OperatorMonoid, OperatorMonoid)>;

  private:
    int n;
    std::vector<Monoid> seg;
    std::vector<OperatorMonoid> lazy;

    const F f;
    const G g;
    const H h;
    const Monoid M1;
    const OperatorMonoid OM1;

    void lazyUpdate(int k, int l, int r) {
      if(lazy[k] == OM1) return;

      if(2 * k + 1 < 2 * n - 1) {
        lazy[2 * k + 1] = h(lazy[2 * k + 1], lazy[k]);
        lazy[2 * k + 2] = h(lazy[2 * k + 2], lazy[k]);
      }
      seg[k] = g(seg[k], lazy[k], r - l);
      lazy[k] = OM1;
    }

  public:
    LazySegmentTree(int sz, const F f, const G g, const H h, const Monoid &M1, const OperatorMonoid &OM1) : f(f), g(g), h(h), M1(M1), OM1(OM1) {
      n = 1;
      while(n < sz) n *= 2;
      seg.resize(2 * n - 1, M1);
      lazy.resize(2 * n - 1, OM1);
    }

    void update(int a, int b, Monoid x, int k = 0, int l = 0, int r = -1) {
      if(r < 0) r = n;

      lazyUpdate(k, l, r);

      if(b <= l || r <= a) return;

      if(a <= l && r <= b) {
        lazy[k] = h(lazy[k], x);
        lazyUpdate(k, l, r);
      }
      else {
        update(a, b, x, 2 * k + 1, l, (l + r) / 2);
        update(a, b, x, 2 * k + 2, (l + r) / 2, r);
        seg[k] = f(seg[k * 2 + 1], seg[k * 2 + 2]);
      }
    }

    Monoid query(int a, int b, int k = 0, int l = 0, int r = -1) {
      if(r < 0) r = n;

      lazyUpdate(k, l, r);

      if(b <= l || r <= a) return M1;
      if(a <= l && r <= b) return seg[k];
      Monoid vl = query(a, b, 2 * k + 1, l, (l + r) / 2);
      Monoid vr = query(a, b, 2 * k + 2, (l + r) / 2, r);
      return f(vl, vr);
    }
};
