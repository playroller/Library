#include <vector>
#include <iostream>

class WarshallFloyd {
    private:
        std::vector<std::vector<int>> d;
        int v;

    public:
        WarshallFloyd(int n) : v(n), d(n, std::vector<int>(n, 1e9)) {
            for(int i = 0; i < v; ++i) d[i][i] = 0;
        }

        void add(int from, int to, int cost) {
            d[from][to] = cost;
            d[to][from] = cost;
        }

        void warshall_froyd() {
            for(int k = 0; k < v; ++k) {
                for(int i = 0; i < v; ++i) {
                    for(int j = 0; j < v; ++j) {
                        d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
                    }
                }
            }
        }

        int dist(int i, int j) {
            return d[i][j];
        }
};