#include <vector>
#include <iostream>

std::vector<std::vector<int>> dist;

void init(int n) {
  dist = std::vector<std::vector<int>>(n, std::vector<int>(n, 1e9));
  for(int i = 0; i < n; ++i) dist[i][i] = 0;
}

void add(int from, int to, int cost) {
  dist[from][to] = cost;
  dist[to][from] = cost;
}

void warshall_froyd() {
  int v = dist.size();
  for(int k = 0; k < v; ++k) {
    for(int i = 0; i < v; ++i) {
      for(int j = 0; j < v; ++j) {
        dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
}