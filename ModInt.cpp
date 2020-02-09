#include <cstdint>
#include <utility>
#include <ostream>
#include <istream>

template <std::int_fast64_t MOD> class ModInt {
  private:
    std::int_fast64_t value;

  public:
    ModInt() : value(0) {}

    ModInt(std::int_fast64_t x) : value(x % MOD) {
      if(value < 0) value += MOD;
    }

    ModInt &operator+=(const ModInt &x) {
      value += x.value;
      if(value >= MOD) value -= MOD;
      return *this;
    }

    ModInt &operator-=(const ModInt &x) {
      if(value < x.value) value += MOD;
      value -= x.value;
      return *this;
    }

    ModInt &operator*=(const ModInt &x) {
      value = value * x.value % MOD;
      return *this;
    }

    ModInt &operator/=(const ModInt &x) {
      *this *= x.inverse();
      return *this;
    }

    ModInt operator-() const { return ModInt(-value); }

    ModInt operator+(const ModInt &x) const { return ModInt(*this) += x; }

    ModInt operator-(const ModInt &x) const { return ModInt(*this) -= x; }

    ModInt operator*(const ModInt &x) const { return ModInt(*this) *= x; }

    ModInt operator/(const ModInt &x) const { return ModInt(*this) /= x; }

    bool operator==(const ModInt *x) const { return value == x.value; }

    bool operator!=(const ModInt *x) const { return value != x.value; }

    ModInt inverse() const {
      std::int_fast64_t a = value, b = MOD, u = 1, v = 0, t;
      while(b > 0) {
        t = a / b;
        std::swap(a -= t * b, b);
        std::swap(u -= t * v, v);
      }
      return ModInt(u);
    }

    ModInt pow(int64_t n) const {
      ModInt ret(1), mul(value);
      while(n > 0) {
        if(n & 1) ret *= mul;
        mul *= mul;
        n >>= 1;
      }
      return ret;
    }

    friend std::ostream &operator<<(std::ostream &os, const ModInt &x) {
      return os << x.value;
    }

    friend std::istream &operator>>(std::istream &is, ModInt &x) {
      std::int_fast64_t t;
      is >> t;
      x = ModInt<MOD>(t);
      return is;
    }

    static std::int_fast64_t get_mod() { return MOD; }
};