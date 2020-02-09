#include <map>

using i64 = long long;

std::map<i64, int> prime_factor(i64 n) {
  std::map<i64, int> ret;
  for(i64 i = 2; i * i <= n; ++i) {
    while(n % i == 0) {
      ret[i]++;
      n /= i;
    }
  }
  if(n != 1) ret[n] = 1;
  return ret;
}