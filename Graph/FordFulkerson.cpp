#include <vector>

template <class T> class FordFulkerson {
  private:
    struct Edge {
      int to, rev;
      T cap;

      Edge(int to, T cap, int rev) : to(to), cap(cap), rev(rev) {}
    };

    std::vector<std::vector<Edge>> edges;
    std::vector<bool> used;

  public:
    FordFulkerson(int v) {
      edges = std::vector<std::vector<Edge>>(v);
      used = std::vector<bool>(v);
    }

    void add_edge(int from, int to, T cap) {
      edges[from].emplace_back(to, cap, edges[to].size());
      edges[to].emplace_back(from, 0, edges[from].size() - 1);
    }

    T dfs(int v, int t, T f) {
      if(v == t) return f;
      used[v] = true;

      for(auto &e : edges[v]) {
        if(!used[e.to] && e.cap > 0) {
          int d = dfs(e.to, t, min(f, e.cap));
          if(d > 0) {
            e.cap -= d;
            edges[e.to][e.rev].cap += d;
            return d;
          }
        }
      }
      return 0;
    }

    T max_flow(int s, int t) {
      int flow = 0;
      while(true) {
        for(int i = 0; i < used.size(); ++i) used[i] = false;

        int f = dfs(s, t, 1e9);
        if(f == 0) return flow;
        flow += f;
      }
    }
};