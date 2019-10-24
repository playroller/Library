#include <vector>
#include <queue>
#include <limits>

template <class T> class Dinic {
  private:
    struct Edge {
      int to, rev;
      T cap;

      Edge(int to, T cap, int rev) : to(to), cap(cap), rev(rev) {}
    };
    
    std::vector<std::vector<Edge>> graph;
    std::vector<int> level;
    std::vector<int> iter;
    
    void bfs(int s) {
      level.assign(level.size(), -1);

      std::queue<int> que;
      level[s] = 0;
      que.push(s);

      while(!que.empty()) {
        int v = que.front();
        que.pop();

        for(auto &e : graph[v]) {
          if(e.cap > 0 && level[e.to] < 0) {
            level[e.to] = level[v] + 1;
            que.push(e.to);
          }
        }
      }
    }

    T dfs(int v, int t, int f) {
      if(v == t) return f;

      for(int &i = iter[v]; i < graph[v].size(); ++i) {
        Edge &e = graph[v][i];

        if(e.cap > 0 && level[v] < level[e.to]) {
          int d = dfs(e.to, t, min(f, e.cap));
          if(d > 0) {
            e.cap -= d;
            graph[e.to][e.rev].cap += d;
            return d;
          }
        }
      }
      return 0;
    }

  public:
    Dinic(int v) {
      graph = std::vector<std::vector<Edge>>(v);
      level = std::vector<int>(v);
      iter = std::vector<int>(v);
    }

    void add_edge(int from, int to, T cap) {
      graph[from].emplace_back(to, cap, graph[to].size());
      graph[to].emplace_back(from, 0, graph[from].size() - 1);
    }

    T max_flow(int s, int t) {
      T flow = 0;

      while(true) {
        bfs(s);

        if(level[t] < 0) return flow;
        iter.assign(iter.size(), 0);

        int f;
        while((f = dfs(s, t, std::numeric_limits<T>::max())) > 0) flow += f;
      }
    }
};