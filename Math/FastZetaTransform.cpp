#include <vector>

std::vector<int> FastZetaTransform(std::vector<int> v) {
    int n = v.size();
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < (1 << n); ++j) {
            if((j & (1 << i)) == 0) v[j] += v[j | (1 << i)];
        }
    }
    return v;
}