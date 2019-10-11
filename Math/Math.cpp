#include <map>
#include <vector>
#include <algorithm>

using i64 = long long;

i64 power(i64 x, i64 n, i64 mod) {
    i64 ret = 1;
    while(n > 0) {
        if(n & 1) (ret *= x) %= mod;
        (x *= x) %= mod;
        n >>= 1;
    }
    return ret;
}

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

bool is_prime(i64 x) {
    for(i64 i = 2; i * i <= x; ++i) {
        if(x % i == 0) return false;
    }
    return true;
}

std::vector<bool> prime_table(int n) {
    std::vector<bool> prime(n + 1, true);
    if(n >= 0) prime[0] = false;
    if(n >= 1) prime[1] = false;
    for(int i = 2; i * i <= n; ++i) {
        if(!prime[i]) continue;
        for(int j = i + i; j <= n; j += i) {
            prime[j] = false;
        }
    }
    return prime;
}

i64 gcd(i64 a, i64 b) { return b == 0 ? a : gcd(b, a % b); }
i64 lcd(i64 a, i64 b) { return a / gcd(a, b) * b; }