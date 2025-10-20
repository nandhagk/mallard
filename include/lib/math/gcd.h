#ifndef MALLARD_MATH_GCD_H
#define MALLARD_MATH_GCD_H

#include <utility>

namespace mld {
template <typename EuclideanRing>
constexpr EuclideanRing gcd(EuclideanRing a, EuclideanRing b) {
    for (EuclideanRing ZERO{}; b != ZERO; a %= b, std::swap(a, b)) {}
    return a;
}
} // namespace mld

#endif // MALLARD_MATH_GCD_H
