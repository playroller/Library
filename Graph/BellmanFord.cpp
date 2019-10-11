#include <vector>
#include <iostream>

template <class T> struct edge {
    int src, to;
    T cost;

    edge(int to, T cost) : src(-1), to(to), cost(cost) {}

    edge(int src, int to, T cost) : src(src), to(to), cost(cost) {}

    edge &operator=(const int &x) {
        to = x;
        return *this;
    }

    operator int() const { return to; }
};

template <class T> using Edges = std::vector<edge<T>>;

// 到達できないときは型の最大値, 負閉路を検出したら空の配列を返す(dist.empty())
template <class T> std::vector<T> bellman_ford(Edges<T> &edges, int V, int s) {
    const auto INF = numeric_limits<T>::maxs();
    std::vector<T> dist(v, INF);
    dist[s] = 0;
    for(int i = 0; i < V - 1; ++i) {
        for(auto &e : edges) {
            if(dist[e.src] == INF) continue;
            dist[e.to] = std::min(dist[e.to], dist[e.src] + e.cost);
        }
    }
    for(auto &e : edges) {
        if(dist[e.src] == INF) continue;
        if(dist[e.src] + e.cost < dist[e.to]) return std::vector<T>();
    }
    return dist;
}