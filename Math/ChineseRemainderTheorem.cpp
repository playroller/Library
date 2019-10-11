#include <vector>
#include <utility>

long long extgcd(long long a, long long b, long long &x, long long &y) {
  if(b == 0) {
    x = 1; y = 0;
    return a;
  }
  long long d = extgcd(b, a % b, y, x);
  y -= a / b * x;
  return d;
}

inline long long MOD(long long a, long long m) {
  return (a % m + m) % m;
}

std::pair<long long, long long> ChineseRem(const std::vector<long long> &b, const std::vector<long long> &m) {
  long long r = 0, M = 1;
  for(int i = 0; i < b.size(); ++i) {
    long long p, q;
    long long d = extgcd(M, m[i], p, q);
    if((b[i] - r) % d != 0) return std::make_pair(0, -1);
    long long tmp = (b[i] - r) / d * p % (m[i] / d);
    r += M * tmp;
    M *= m[i] / d;
  }
  return std::make_pair(MOD(r, M), M);
}