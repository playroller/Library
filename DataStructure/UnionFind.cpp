#include <vector>

class UnionFind {
    private:
      std::vector<int> par;
      std::vector<int> sizes;
      std::vector<int> rank;
      int group;

    public:
      UnionFind(int n) : par(n), sizes(n, 1), rank(n, 0), group(n) {
        for(int i = 0; i < n; ++i) par[i] = i;
      }

      int find(int x) {
        if(x == par[x]) return x;
        return par[x] = find(par[x]);
      }

      void unite(int x, int y) {
        x = find(x);
        y = find(y);

        if(x == y) return;

        if(rank[x] < rank[y]) std::swap(x, y);
        par[y] = x;
        sizes[x] += sizes[y];
        if(rank[x] == rank[y]) rank[x]++;
        group--;
      }

      bool same(int x, int y) {
        return find(x) == find(y);
      }

      int size(int x) {
        return sizes[find(x)];
      }

      int groupSize() {
        return group;
      }
};