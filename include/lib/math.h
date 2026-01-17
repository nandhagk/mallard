#ifndef MALLARD_MATH_H
#define MALLARD_MATH_H

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>

#include "lib/prelude.h"
#include "lib/type_traits/numeric.h"

namespace mld {
template <std::integral T>
[[nodiscard]] constexpr T floor(T a, T b) noexcept {
    return a / b - (a % b && (a ^ b) < 0);
}

template <std::integral T>
[[nodiscard]] constexpr T ceil(T a, T b) noexcept {
    return floor(a + b - 1, a);
}

template <typename T, std::unsigned_integral Scalar,
          std::invocable<T, T> F = std::multiplies<>>
[[nodiscard]] constexpr T pow(T a, Scalar n, F mul = F(),
                              T one = static_cast<T>(1)) noexcept {
    T r = one;
    for (; n != 0; n >>= 1, a = mul(a, a))
        if (n & 1) r = mul(r, a);

    return r;
}

template <typename T>
[[nodiscard]] constexpr T gcd(T a, T b, T zero = static_cast<T>(0)) noexcept {
    for (; b != zero; a %= b, std::swap(a, b));
    return a;
}

template <typename T>
[[nodiscard]] constexpr T lcm(T a, T b, T zero = static_cast<T>(0)) noexcept {
    return a / gcd(a, b, zero) * b;
}

template <std::integral T, std::unsigned_integral U>
[[nodiscard]] constexpr U safe_mod(T x, U m) noexcept {
    x %= m;
    return static_cast<U>(x < 0 ? x + m : x);
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr std::pair<U, U> inv_gcd(U a, U m) noexcept {
    assert(a < m);
    if (a == 0) return {m, 0};

    using T = std::make_signed_t<U>;

    U s = m, t = a;
    T x = 0, y = 1;
    for (; t != 0; std::swap(s, t), std::swap(x, y)) {
        U u = s / t;
        s -= t * u;
        x -= y * u;
    }

    if (x < 0) x += m / s;
    return {s, static_cast<U>(x)};
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr U mod_inv(U a, U m) noexcept {
    const auto [x, y] = inv_gcd(a, m);
    assert(x == 1);

    return y;
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr U mod_mul(U a, U b, U m) noexcept {
    return static_cast<U>(make_double_width_t<U>(a) * b % m);
}

template <std::unsigned_integral U, std::unsigned_integral Scalar>
[[nodiscard]] constexpr U mod_pow(U a, Scalar n, U m) noexcept {
    assert(a < m);
    return pow(a, n, [m](U x, U y) { return mod_mul(x, y, m); }, U{1} % m);
}
} // namespace mld

#endif // MALLARD_MATH_H
