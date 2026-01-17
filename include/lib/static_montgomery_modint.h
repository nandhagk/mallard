#ifndef MALLARD_STATIC_MONTGOMERY_MODINT_H
#define MALLARD_STATIC_MONTGOMERY_MODINT_H 1

#include <concepts>
#include <iostream>
#include <limits>
#include <type_traits>

#include "lib/math.h"
#include "lib/miller_rabin.h"
#include "lib/prelude.h"
#include "lib/type_traits/numeric.h"

namespace mld {
template <std::unsigned_integral U, U m>
struct static_montgomery_modint_base {
    using mint = static_montgomery_modint_base;

    using V = make_double_width_t<U>;
    using S = std::make_signed_t<U>;

    static constexpr u32 W = std::numeric_limits<U>::digits;
    static_assert(m & 1 && m <= U(1) << (W - 2));

    constexpr static U get_r() {
        U p = m;
        for (; m * p != 1; p *= static_cast<U>(2) - m * p);
        return p;
    }

    static constexpr U n2 = static_cast<U>(-static_cast<V>(m) % m);
    static constexpr U r = get_r();

    constexpr static_montgomery_modint_base()
        : v(0) {}

    template <std::integral T>
    constexpr static_montgomery_modint_base(T x)
        : v(reduce(static_cast<V>(x % m + m) * n2)) {}

    constexpr static U reduce(V b) {
        return static_cast<U>(
            (b + static_cast<V>(static_cast<U>(b) * static_cast<U>(-r)) * m) >> W);
    }

    constexpr static U mod() {
        return m;
    }

    constexpr U val() const {
        U p = reduce(v);
        return p >= m ? p - m : p;
    }

    template <std::unsigned_integral Scalar>
    constexpr mint pow(Scalar n) const {
        return mld::pow(*this, n);
    }

    constexpr mint inv() const {
        if constexpr (is_prime)
            return pow(mod() - 2);
        else
            return mld::mod_inv(val(), mod());
    }

    constexpr mint &operator+=(const mint &rhs) & {
        if (static_cast<S>(v += rhs.v - 2 * m) < 0) v += 2 * m;
        return *this;
    }

    constexpr mint &operator-=(const mint &rhs) & {
        if (static_cast<S>(v -= rhs.v) < 0) v += 2 * m;
        return *this;
    }

    constexpr mint &operator*=(const mint &rhs) & {
        v = reduce(static_cast<V>(v) * rhs.v);
        return *this;
    }

    constexpr mint &operator/=(const mint &rhs) & {
        return *this *= rhs.inv();
    }

    constexpr friend mint operator+(mint lhs, const mint &rhs) {
        return lhs += rhs;
    }

    constexpr friend mint operator-(mint lhs, const mint &rhs) {
        return lhs -= rhs;
    }

    constexpr friend mint operator*(mint lhs, const mint &rhs) {
        return lhs *= rhs;
    }

    constexpr friend mint operator/(mint lhs, const mint &rhs) {
        return lhs /= rhs;
    }

    constexpr mint operator-() const {
        return mint(0) - mint(*this);
    }

    constexpr friend bool operator==(const mint &lhs, const mint &rhs) {
        return (lhs.v >= m ? lhs.v - m : lhs.v) == (rhs.v >= m ? rhs.v - m : rhs.v);
    }

    constexpr friend bool operator!=(const mint &lhs, const mint &rhs) {
        return !(lhs == rhs);
    }

private:
    U v;
    inline static constexpr bool is_prime = is_prime_v<U, m>;
};

template <u32 m>
using static_montgomery_modint_32 = static_montgomery_modint_base<u32, m>;

template <u64 m>
using static_montgomery_modint_64 = static_montgomery_modint_base<u64, m>;

using montgomerymodint998244353 = static_montgomery_modint_32<998'244'353>;
using montgomerymodint1000000007 = static_montgomery_modint_32<1'000'000'007>;
} // namespace mld

#endif // MALLARD_STATIC_MONTGOMERY_MODINT_H
