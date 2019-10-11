const int sz = 510000;

using i64 = long long;
const int mod = 1e9 + 7;

i64 fac[sz], finv[sz], inv[sz];

void init() {
  fac[0] = fac[1] = 1;
  finv[0] = finv[1] = 1;
  inv[1] = 1;

  for(i64 i = 2; i < sz; ++i) {
    fac[i] = fac[i - 1] * i % mod;
    inv[i] = mod - inv[mod % i] * (mod / i) % mod;
    finv[i] = finv[i - 1] * inv[i] % mod;
  }
}

i64 get(int n, int k) {
  return fac[n] * (finv[k] * finv[n - k] % mod) % mod;
}