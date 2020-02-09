using i64 = long long;

bool is_prime(i64 x) {
  for(i64 i = 2; i * i <= x; ++i) {
    if(x % i == 0) return false;
  }
  return true;
}