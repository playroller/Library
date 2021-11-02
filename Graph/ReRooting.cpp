#include <vector>
#include <functional>

template <class Monoid> class ReRooting {
  private:
		using Graph = std::vector<std::vector<int>>;

		using F1 = std::function<Monoid(Monoid, Monoid)>;
		using F2 = std::function<Monoid(Monoid, int)>;

    const int V;
		const F1 merge;
		const F2 f, g;
		const Monoid M1;

		std::vector<std::vector<Monoid>> dp;
		Graph G;

    Monoid dfs1(int p, int v) {
      Monoid res = M1;
      for(int i = 0; i < G[v].size(); ++i) {
        if(G[v][i] == p) continue;

        dp[v][i] = dfs1(v, G[v][i]);
        res = merge(res, f(dp[v][i], G[v][i]));
      }
      return g(res, v);
    }

    void dfs2(int p, int v, const Monoid &from_par) {
      for(int i = 0; i < G[v].size(); ++i) {
        if(G[v][i] == p) {
          dp[v][i] = from_par;
          break;
        }
      }

      std::vector<Monoid> pR(G[v].size() + 1);
      pR[G[v].size()] = M1;
      for(int i = G[v].size(); i > 0; --i) {
        pR[i - 1] = merge(pR[i], f(dp[v][i - 1], G[v][i - 1]));
      }
      Monoid pL = M1;
      for(int i = 0; i < G[v].size(); ++i) {
        if(G[v][i] != p) {
          Monoid val = merge(pL, pR[i + 1]);
          dfs2(v, G[v][i], g(val, v));
        }
        pL = merge(pL, f(dp[v][i], G[v][i]));
      }
    }

	public:
		ReRooting(int V, F1 merge, F2 f, F2 g, const Monoid &M1) : V(V), merge(merge), f(f), g(g), M1(M1) {
			G.resize(V);
      dp.resize(V);
		}

    void add_edge(int a, int b) {
      G[a].emplace_back(b);
      G[b].emplace_back(a);
    }

    std::vector<Monoid> rerooting(int root = 0) {
      for(int i = 0; i < V; ++i) {
        dp[i].resize(G[i].size());
      }
      dfs1(-1, root);
      dfs2(-1, root, M1);

      std::vector<Monoid> ans(V, M1);
      for(int v = 0; v < V; ++v) {
        for(int i = 0; i < G[v].size(); ++i) {
          ans[v] = merge(ans[v], f(dp[v][i], G[v][i]));
        }
        ans[v] = g(ans[v], v);
      }
      return ans;
    }
};