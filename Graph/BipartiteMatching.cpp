#include <vector>

class BipartiteMatching {
  private:
    std::vector<std::vector<int>> graph;
    std::vector<int> match;
    std::vector<bool> used;

    bool dfs(int v) {
      used[v] = true;

      for(auto &e : graph[v]) {
        int m = match[e];

        if(m < 0 || (!used[m] && dfs(m))) {
          match[v] = e;
          match[e] = v;
          return true;
        }
      }
      return false;
    }

  public:
    BipartiteMatching(int v) {
      graph = std::vector<std::vector<int>>(v);
      match = std::vector<int>(v, -1);
      used = std::vector<bool>(v);
    }

    void add_edge(int u, int v) {
      graph[u].push_back(v);
      graph[v].push_back(u);
    }

    int bipartite_matching() {
      int res = 0;

      for(int v = 0; v < graph.size(); ++v) {
        if(match[v] < 0) {
          used.assign(used.size(), false);
          if(dfs(v)) res++;
        }
      }
      return res;
    }
};