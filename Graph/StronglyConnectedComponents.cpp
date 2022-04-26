#include <vector>
#include <pair>
#include <utility>
#include <algorithm>

class StronglyConnectedComponents {
  private:
    int sz;
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> rgraph;
    std::vector<int> order;
    std::vector<int> component;
    std::vector<bool> used;
    std::vector<int> component_size;

    void dfs(int v) {
      used[v] = true;
      for(auto &nxt : graph[v]) {
        if(!used[nxt]) dfs(nxt);
      }
      order.push_back(v);
    }

    void rdfs(int v, int k) {
      component[v] = k;
      for(auto &nxt : rgraph[v]) {
        if(component[nxt] == -1) rdfs(nxt, k);
      }
    }

  public:
    StronglyConnectedComponents(std::vector<std::vector<int>> &_graph) {
      sz = _graph.size();
      graph = _graph;
      rgraph = std::vector<std::vector<int>>(sz);
      order.reserve(sz);
      component = std::vector<int>(sz, -1);
      used = std::vector<bool>(sz, false);

      for(int v = 0; v < sz; ++v) {
        for(auto &nxt : graph[v]) rgraph[nxt].push_back(v);
      }

      for(int v = 0; v < sz; ++v) {
        if(!used[v]) dfs(v);
      }

      int k = 0;
      std::reverse(order.begin(), order.end());
      for(auto &v : order) {
        if(component[v] == -1) rdfs(v, k), ++k;
      }
    }

    bool same(int a, int b) {
      return component[a] == component[b];
    }

    std::vector<int> get_component_size() {
      if(component_size.size() > 0) return component_size;

      int n = *std::max_element(component.begin(), component.end()) + 1;
      component_size = std::vector<int>(n, 0);

      for(int v = 0; v < sz; ++v) {
        component_size[component[v]]++;
      }
      return component_size;
    }

    std::vector<std::vector<int>> rebuild() {
      int n = *std::max_element(component.begin(), component.end()) + 1;
      std::vector<std::vector<int>> newgraph(n);
      std::set<std::pair<int, int>> connected;

      for(int v = 0; v < n; ++v) {
        for(auto &nxt : graph[v]) {
          if(component[v] != component[nxt] && !connected.count(std::make_pair(v, nxt))) {
            newgraph[component[v]].push_back(component[nxt]);
            connected.insert(std::make_pair(v, nxt));
          }
        }
      }
      return newgraph;
    }
};