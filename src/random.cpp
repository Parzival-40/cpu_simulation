#include "random.h"

std::random_device Random::rd;
double Random::expo(double mean) {
  double r = rd() / (double)rd.max();
  double ex = -mean * log(r);
  return ex;
}
double Random::unif(double a, double b) {
  double r = rd() / (double)rd.max();
  return a + (b - a) * r;
}
double Random::gamma(double a, double b, double p, double q) {
  std::gamma_distribution<double> dist(a, b);
  double mx = 0;
  for (int i = 0; i < 1000; i++) {
    mx = std::max(mx, dist(rd));
  }
  double r = 0;
  do {
    r = dist(rd);
  } while (r >= mx);
  return p + r / mx * q;
}
