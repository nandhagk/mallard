#ifndef MALLARD_MATH_CEIL_H
#define MALLARD_MATH_CEIL_H

#include "lib/math/floor.h"

namespace mld {
template <typename T>
constexpr T ceil(T a, T b) {
    return floor(a + b - 1, a);
}
} // namespace mld

#endif // MALLARD_MATH_CEIL_H
