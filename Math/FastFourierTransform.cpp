#include <vector>
#include <complex>

class FastFourierTransform {
  private:
    using Real = long double;
    using Complex = std::complex<Real>;
    using i64 = long long;
 
    std::vector<Complex> fft(std::vector<Complex> a, bool inverse) {
      constexpr Real PI = 3.14159265358979323846;
 
      int n = a.size(), mask = n - 1;
      std::vector<Complex> tmp(n);
 
      for(int i = n >> 1; i >= 1; i >>= 1) {
        Real theta = 2 * PI * i * (inverse ? -1 : 1) / n;
        Complex zeta(std::cos(theta), std::sin(theta));
        Complex powZeta = 1; 
 
        for(int j = 0; j < n; j += i) {
          for(int k = 0; k < i; ++k) {
            tmp[j + k] = a[((j << 1) & mask) | k] + powZeta * a[(((j << 1) | i) & mask) | k];
          }
          powZeta *= zeta;
        }
        swap(a, tmp);
      }
 
      if(inverse) {
        for(int i = 0; i < n; ++i) a[i] /= n;
      }
      return a;
    }
 
 
  public:
    std::vector<i64> convolution(std::vector<i64> &ar, std::vector<i64> &br) {
      int deg = ar.size() + br.size() - 1;
 
      int n = 1;
      while(n < deg) n <<= 1;
 
      std::vector<Complex> a(n);
      for(int i = 0; i < n; ++i) a[i] = 0;
      for(int i = 0; i < ar.size(); ++i) a[i].real(ar[i]);
      for(int i = 0; i < br.size(); ++i) a[i].imag(br[i]);
 
      a = fft(a, false);
 
      std::vector<Complex> c(n);
      for(int i = 0; i < n; ++i) {
        int j = i == 0 ? 0 : n - i;
        c[i] = (a[i] + std::conj(a[j])) * (a[i] - conj(a[j])) * Complex(0, -.25);
      }
 
      c = fft(c, true);
 
      std::vector<i64> res(n);
      for(int i = 0; i < n; ++i) {
        res[i] = std::round(c[i].real());
      }
      return res;
    }
};