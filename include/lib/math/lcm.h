#ifndef MALLARD_MATH_LCM_H
#define MALLARD_MATH_LCM_H

#include "lib/math/gcd.h"

namespace mld {
template <typename EuclideanRing>
constexpr EuclideanRing lcm(EuclideanRing a, EuclideanRing b) {
    return a / gcd(a, b) * b;
}
} // namespace mld

#endif // MALLARD_MATH_LCM_H
