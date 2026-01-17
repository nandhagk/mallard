#ifndef MALLARD_GAUSSIAN_INTEGER_H
#define MALLARD_GAUSSIAN_INTEGER_H

#include <iostream>

#include "lib/math.h"

namespace mld {

template <std::signed_integral T>
struct gaussian_integer {
    using gint = gaussian_integer;

    T x, y;

    constexpr gaussian_integer() noexcept
        : gaussian_integer(0, 0) {}

    explicit constexpr gaussian_integer(T a) noexcept
        : gaussian_integer(a, 0) {}

    explicit constexpr gaussian_integer(const std::pair<T, T> &p) noexcept
        : gaussian_integer(p.first, p.second) {}

    constexpr gaussian_integer(T a, T b) noexcept
        : x{a}, y{b} {}

    constexpr gaussian_integer(const gint &other) noexcept = default;
    constexpr gint &operator=(const gint &other) noexcept = default;
    constexpr gaussian_integer(gint &&other) noexcept = default;
    constexpr gint &operator=(gint &&other) noexcept = default;

    [[nodiscard]] constexpr T norm() const noexcept {
        return x * x + y * y;
    }

    [[nodiscard]] constexpr gint conjugate() const noexcept {
        return gint(x, -y);
    }

    constexpr gint &operator+=(const gint &rhs) & noexcept {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr gint &operator-=(const gint &rhs) & noexcept {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    constexpr gint &operator*=(const gint &rhs) & noexcept {
        const auto [a, b] = *this;
        x = a * rhs.x - b * rhs.y;
        y = a * rhs.y + b * rhs.x;
        return *this;
    }

    constexpr gint &operator/=(const gint &rhs) & noexcept {
        const T n = rhs.norm();
        *this *= rhs.conjugate();
        x = floor(x + n / 2, n);
        y = floor(y + n / 2, n);
        return *this;
    }

    constexpr gint &operator%=(const gint &rhs) & noexcept {
        *this -= (*this / rhs) * rhs;
        return *this;
    }

    [[nodiscard]] constexpr gint operator-() const noexcept {
        return gint(-x, -y);
    }

    [[nodiscard]] friend constexpr gint operator+(gint lhs, const gint &rhs) noexcept {
        return lhs += rhs;
    }

    [[nodiscard]] friend constexpr gint operator-(gint lhs, const gint &rhs) noexcept {
        return lhs -= rhs;
    }

    [[nodiscard]] friend constexpr gint operator*(gint lhs, const gint &rhs) noexcept {
        return lhs *= rhs;
    }

    [[nodiscard]] friend constexpr gint operator/(gint lhs, const gint &rhs) noexcept {
        return lhs /= rhs;
    }

    [[nodiscard]] friend constexpr gint operator%(gint lhs, const gint &rhs) noexcept {
        return lhs %= rhs;
    }

    [[nodiscard]] friend constexpr bool operator==(const gint &lhs,
                                                   const gint &rhs) noexcept {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    [[nodiscard]] friend constexpr bool operator!=(const gint &lhs,
                                                   const gint &rhs) noexcept {
        return !(lhs == rhs);
    }
};
} // namespace mld

#endif // MALLARD_GAUSSIAN_INTEGER_H
