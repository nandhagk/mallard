#ifndef MALLARD_MATH_FLOOR_H
#define MALLARD_MATH_FLOOR_H

namespace mld {
template <typename T>
constexpr T floor(T a, T b) {
    return a / b - (a % b && (a ^ b) < 0);
}
} // namespace mld

#endif // MALLARD_MATH_FLOOR_H
