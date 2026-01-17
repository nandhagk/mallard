#ifndef MALLARD_STATIC_MODINT_H
#define MALLARD_STATIC_MODINT_H 1

#include <iostream>

#include "lib/math.h"
#include "lib/miller_rabin.h"
#include "lib/prelude.h"

namespace mld {
template <std::unsigned_integral U, U m>
struct static_modint_base {
    using mint = static_modint_base;

    constexpr static_modint_base() noexcept
        : v(0) {}

    template <std::unsigned_integral V>
    constexpr static_modint_base(V x) noexcept
        : v(static_cast<U>(x % mod())) {}

    template <std::signed_integral T>
    constexpr static_modint_base(T x) noexcept
        : v(safe_mod(x, mod())) {}

    [[nodiscard]] static constexpr U mod() noexcept {
        return m;
    }

    [[nodiscard]] constexpr U val() const noexcept {
        return v;
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] constexpr mint pow(Scalar n) const noexcept {
        return mld::pow(*this, n);
    }

    [[nodiscard]] constexpr mint inv() const noexcept {
        if constexpr (is_prime)
            return pow(mod() - 2);
        else
            return mld::mod_inv(val(), mod());
    }

    constexpr mint &operator+=(const mint &rhs) & noexcept {
        if ((v += rhs.val()) >= mod()) v -= mod();
        return *this;
    }

    constexpr mint &operator-=(const mint &rhs) & noexcept {
        if ((v += mod() - rhs.val()) >= mod()) v -= mod();
        return *this;
    }

    constexpr mint &operator*=(const mint &rhs) & noexcept {
        v = mod_mul(v, rhs.val(), mod());
        return *this;
    }

    constexpr mint &operator/=(const mint &rhs) & noexcept {
        return *this *= rhs.inv();
    }

    [[nodiscard]] friend constexpr mint operator+(mint lhs, const mint &rhs) noexcept {
        return lhs += rhs;
    }

    [[nodiscard]] friend constexpr mint operator-(mint lhs, const mint &rhs) noexcept {
        return lhs -= rhs;
    }

    [[nodiscard]] friend constexpr mint operator*(mint lhs, const mint &rhs) noexcept {
        return lhs *= rhs;
    }

    [[nodiscard]] friend constexpr mint operator/(mint lhs, const mint &rhs) noexcept {
        return lhs /= rhs;
    }

    [[nodiscard]] constexpr mint operator-() const noexcept {
        return mint(0) - mint(*this);
    }

    [[nodiscard]] friend constexpr bool operator==(const mint &lhs,
                                                   const mint &rhs) noexcept {
        return lhs.val() == rhs.val();
    }

    [[nodiscard]] friend constexpr bool operator!=(const mint &lhs,
                                                   const mint &rhs) noexcept {
        return lhs.val() != rhs.val();
    }

private:
    U v;
    inline static constexpr bool is_prime = is_prime_v<U, m>;
};

template <u32 m>
using static_modint_32 = static_modint_base<u32, m>;

template <u64 m>
using static_modint_64 = static_modint_base<u64, m>;

using modint998244353 = static_modint_32<998'244'353>;
using modint1000000007 = static_modint_32<1'000'000'007>;
} // namespace mld

#endif // MALLARD_STATIC_MODINT_H
