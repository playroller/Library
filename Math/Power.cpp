using i64 = long long;

i64 pow(i64 x, i64 n, i64 mod) {
  i64 ret = 1;
  while(n > 0) {
    if(n & 1) (ret *= x) %= mod;
    (x *= x) %= mod;
    n >>= 1;
  }
  return ret;
}