#include <vector>

template <class Abel> class WeightedUnionFind {
  private:
    std::vector<int> par;
    std::vector<int> rank;
    std::vector<Abel> diff_weight;

  public:
    WeightedUnionFind(int n, Abel SUM_UNITY = 0) : par(n), rank(n, 0), diff_weight(n, SUM_UNITY) {
      for(int i = 0; i < n; ++i) par[i] = i;
    }

    Abel weight(int x) {
      find(x);
      return diff_weight[x];
    }

    int find(int x) {
      if(x == par[x]) return x;

      int r = find(par[x]);
      diff_weight[x] += diff_weight[par[x]];
      return par[x] = r;
    }

    void unite(int x, int y, Abel w) {
      w += weight(x);
      w -= weight(y);
      x = find(x);
      y = find(y);

      if(x == y) return;
      if(rank[x] < rank[y]) {
        std::swap(x, y);
        w = -w;
      }
      par[y] = x;
      diff_weight[y] = w;
      if(rank[x] == rank[y]) rank[x]++;
    }

    bool same(int x, int y) {
      return find(x) == find(y);
    }

    Abel diff(int x, int y) {
      return weight(y) - weight(x);
    }
};