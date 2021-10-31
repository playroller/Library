#include <vector>
#include <functional>

template <class Monoid> class DualSegmentTree {
  using F = std::function<Monoid(Monoid, Monoid)>;

  private:
    int n;
    std::vector<Monoid> seg;

    const F f;
    const Monoid M1;

		Monoid get(int k) {
			Monoid res = M1;

			while(k > 0) {
				res = f(res, seg[k]);
				k  = k >> 1;
			}
			return res;
		}

  public:
    DualSegmentTree(int sz, const F f, const Monoid &M1) : f(f), M1(M1) {
      n = 1;
      while(n < sz) n *= 2;
      seg.resize(2 * n, M1);
    }

    void update(int a, int b, Monoid x, int k = 1, int l = 0, int r = -1) {
      if(r < 0) r = n;

      if(b <= l || r <= a) return;

      if(a <= l && r <= b) {
        seg[k] = f(seg[k], x);
      }
      else {
        update(a, b, x, 2 * k + 0, l, (l + r) / 2);
        update(a, b, x, 2 * k + 1, (l + r) / 2, r);
      }
    }

    Monoid operator[](int k) {
			k += n;
			return get(k);
    }

		void set(int k, Monoid x) {
			k += n;
			seg[k] = x;
		}
};