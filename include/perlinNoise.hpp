#pragma once
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>

class PerlinNoise {
public:
  std::vector<int> p;

  PerlinNoise(int seed = 45) {
    p.resize(512);
    std::iota(p.begin(), p.begin() + 256, 0);
    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.begin() + 256, engine);
    std::copy(p.begin(), p.begin() + 256, p.begin() + 256);
  }

  // retourne une valeur dans [0, 1]
  double noise(double x, double y) const {
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    double u = fade(x);
    double v = fade(y);

    int aa = p[p[X]     + Y];
    int ab = p[p[X]     + Y + 1];
    int ba = p[p[X + 1] + Y];
    int bb = p[p[X + 1] + Y + 1];

    double res = lerp(v,
      lerp(u, grad(aa, x,     y),     grad(ba, x - 1, y)),
      lerp(u, grad(ab, x,     y - 1), grad(bb, x - 1, y - 1))
    );

    return (res + 1.0) / 2.0;
  }

private:
  static double lerp(double t, double a, double b) { return a + t * (b - a); }
  static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
  static double grad(int hash, double x, double y) {
    switch (hash & 3) {
      case 0:  return  x + y;
      case 1:  return -x + y;
      case 2:  return  x - y;
      default: return -x - y;
    }
  }
};
