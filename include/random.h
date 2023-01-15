#ifndef RANDOM
#define RANDOM
#include <random>

class Random {
public:
  static std::random_device rd;
  static double expo(double mean);
  static double unif(double a, double b);
  // This gamma function is a workaround of the original gamma in the cpp random
  // library This gamma function takes in 4 parameters a: alpha of the original
  // gamma function b: beta of the original gamma function {p..q} is the range
  // of the output gamma random variable
  static double gamma(double a, double b, double p, double q);
};

#endif