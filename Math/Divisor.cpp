#include <vector>
#include <algorithm>

using i64 = long long;

std::vector<i64> divisor(i64 n) {
  std::vector<i64> ret;
  for(i64 i = 1; i * i <= n; ++i) {
    if(n % i == 0) {
      ret.push_back(i);
      if(i * i != n) ret.push_back(n / i);
    }
  }
  sort(ret.begin(), ret.end());
  return ret;
}