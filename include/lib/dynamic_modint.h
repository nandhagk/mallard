#ifndef MALLARD_DYNAMIC_MODINT_H
#define MALLARD_DYNAMIC_MODINT_H 1

#include <concepts>

#include "lib/barrett.h"
#include "lib/math.h"
#include "lib/prelude.h"

namespace mld {
template <std::unsigned_integral U, i32 id>
struct dynamic_modint_base {
    using mint = dynamic_modint_base;

    constexpr dynamic_modint_base() noexcept
        : v(0) {}

    template <std::unsigned_integral T>
    dynamic_modint_base(T x) noexcept
        : v(U(x % mod())) {}

    template <std::signed_integral T>
    dynamic_modint_base(T x) noexcept {
        using S = std::make_signed_t<U>;

        S u = S(x % S(mod()));
        if (u < 0) u += mod();

        v = u;
    }

    static void set_mod(U m) noexcept {
        bt = m;
    }

    [[nodiscard]] static U mod() noexcept {
        return bt.mod();
    }

    [[nodiscard]] U val() const noexcept {
        return v;
    }

    [[nodiscard]] mint inv() const noexcept {
        return mod_inv(val(), mod());
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] mint pow(Scalar n) const noexcept {
        return mld::pow(*this, n);
    }

    mint &operator+=(const mint &rhs) & noexcept {
        if ((v += rhs.val()) >= mod()) v -= mod();
        return *this;
    }

    mint &operator-=(const mint &rhs) & noexcept {
        if ((v += mod() - rhs.val()) >= mod()) v -= mod();
        return *this;
    }

    mint &operator*=(const mint &rhs) & noexcept {
        v = bt.mul(v, rhs.val());
        return *this;
    }

    mint &operator/=(const mint &rhs) & noexcept {
        return *this *= rhs.inv();
    }

    mint &operator++() & noexcept {
        return *this += 1;
    }

    mint &operator--() & noexcept {
        return *this -= 1;
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

    mint operator++(int) const noexcept {
        mint tmp = *this;
        operator++();
        return tmp;
    }

    mint operator--(int) const noexcept {
        mint tmp = *this;
        operator--();
        return tmp;
    }

    [[nodiscard]] friend bool operator==(const mint &lhs, const mint &rhs) noexcept {
        return lhs.val() == rhs.val();
    }

    [[nodiscard]] friend bool operator!=(const mint &lhs, const mint &rhs) noexcept {
        return lhs.val() != rhs.val();
    }

private:
    U v;
    static barrett_t<U> bt;
};

template <i32 id>
using dynamic_modint_32 = dynamic_modint_base<u32, id>;

template <>
inline barrett_t<u32> dynamic_modint_32<-1>::bt = 998'244'353;

template <i32 id>
using dynamic_modint_64 = dynamic_modint_base<u64, id>;

template <>
inline barrett_t<u64> dynamic_modint_64<-1>::bt = (u64(1) << 61) - 1;
} // namespace mld

#endif // MALLARD_DYNAMIC_MODINT_H
