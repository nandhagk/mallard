#ifndef MALLARD_DYNAMIC_MONTGOMERY_MODINT_H
#define MALLARD_DYNAMIC_MONTGOMERY_MODINT_H 1

#include <concepts>
#include <iostream>
#include <limits>
#include <type_traits>

#include "lib/math.h"
#include "lib/prelude.h"
#include "lib/type_traits/numeric.h"

namespace mld {
template <std::unsigned_integral U, i32 id>
struct dynamic_montgomery_modint_base {
    using mint = dynamic_montgomery_modint_base;

    using V = make_double_width_t<U>;
    using S = std::make_signed_t<U>;

    static constexpr u32 W = std::numeric_limits<U>::digits;

    dynamic_montgomery_modint_base() noexcept
        : v(0) {}

    template <std::integral T>
    dynamic_montgomery_modint_base(T x) noexcept
        : v(reduce(static_cast<V>(x % m + m) * n2)) {}

    static U reduce(V b) noexcept {
        return static_cast<U>(
            (b + static_cast<V>(static_cast<U>(b) * static_cast<U>(-r)) * m) >> W);
    }

    static U get_r() noexcept {
        U p = m;
        for (; m * p != 1; p *= static_cast<U>(2) - m * p);
        return p;
    }

    static void set_mod(U mod) noexcept {
        assert(mod & 1 && mod <= static_cast<U>(1) << (W - 2));

        m = mod;
        n2 = static_cast<U>(-static_cast<V>(m) % m);
        r = get_r();
    }

    [[nodiscard]] static U mod() noexcept {
        return m;
    }

    [[nodiscard]] U val() const noexcept {
        U p = reduce(v);
        return p >= m ? p - m : p;
    }

    [[nodiscard]] mint inv() const noexcept {
        return mld::mod_inv(val(), mod());
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] mint pow(Scalar n) const noexcept {
        return mld::pow(*this, n);
    }

    mint &operator+=(const mint &rhs) & noexcept {
        if (static_cast<S>(v += rhs.v - 2 * m) < 0) v += 2 * m;
        return *this;
    }

    mint &operator-=(const mint &rhs) & noexcept {
        if (static_cast<S>(v -= rhs.v) < 0) v += 2 * m;
        return *this;
    }

    mint &operator*=(const mint &rhs) & noexcept {
        v = reduce(static_cast<V>(v) * rhs.v);
        return *this;
    }

    mint &operator/=(const mint &rhs) & noexcept {
        return *this *= rhs.inv();
    }

    [[nodiscard]] friend mint operator+(mint lhs, const mint &rhs) noexcept {
        return lhs += rhs;
    }

    [[nodiscard]] friend mint operator-(mint lhs, const mint &rhs) noexcept {
        return lhs -= rhs;
    }

    [[nodiscard]] friend mint operator*(mint lhs, const mint &rhs) noexcept {
        return lhs *= rhs;
    }

    [[nodiscard]] friend mint operator/(mint lhs, const mint &rhs) noexcept {
        return lhs /= rhs;
    }

    [[nodiscard]] mint operator-() const noexcept {
        return mint(0) - mint(*this);
    }

    [[nodiscard]] friend bool operator==(const mint &lhs, const mint &rhs) noexcept {
        return (lhs.v >= m ? lhs.v - m : lhs.v) == (rhs.v >= m ? rhs.v - m : rhs.v);
    }

    [[nodiscard]] friend bool operator!=(const mint &lhs, const mint &rhs) noexcept {
        return !(lhs == rhs);
    }

private:
    U v;
    inline static U m, r, n2;
};

template <i32 id>
using dynamic_montgomery_modint_32 = dynamic_montgomery_modint_base<u32, id>;

template <i32 id>
using dynamic_montgomery_modint_64 = dynamic_montgomery_modint_base<u64, id>;
} // namespace mld

#endif // MALLARD_DYNAMIC_MONTGOMERY_MODINT_H
