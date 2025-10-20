#ifndef MALLARD_MATH_GAUSSIAN_INTEGER_H
#define MALLARD_MATH_GAUSSIAN_INTEGER_H

#include <iostream>

#include "lib/math/floor.h"

namespace mld {

template <typename T>
struct gaussian_integer {
    using gint = gaussian_integer;

    T x, y;

    explicit constexpr gaussian_integer(T a = 0, T b = 0)
        : x{a}, y{b} {}

    explicit constexpr gaussian_integer(const std::pair<T, T> &p)
        : gaussian_integer(p.first, p.second) {}

    constexpr gaussian_integer(const gaussian_integer &other) = default;
    constexpr gaussian_integer& operator=(const gaussian_integer &other) = default;
    constexpr gaussian_integer(gaussian_integer &&other) = default;
    constexpr gaussian_integer& operator=(gaussian_integer &&other) = default;

    constexpr T norm() const {
        return x * x + y * y;
    }

    constexpr gint conjugate() const {
        return gint(x, -y);
    }

    constexpr gint &operator+=(const gint &rhs) & {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr gint &operator-=(const gint &rhs) & {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    constexpr gint &operator*=(const gint &rhs) & {
        const auto [a, b] = *this;
        x = a * rhs.x - b * rhs.y;
        y = a * rhs.y + b * rhs.x;
        return *this;
    }

    constexpr gint &operator/=(const gint &rhs) & {
        const T n = rhs.norm();
        *this *= rhs.conjugate();
        x = floor(x + n / 2, n);
        y = floor(y + n / 2, n);
        return *this;
    }

    constexpr gint &operator%=(const gint &rhs) & {
        *this -= (*this / rhs) * rhs;
        return *this;
    }

    constexpr gint operator-() const {
        return gint(-x, -y);
    }

    friend constexpr gint operator+(gint lhs, const gint &rhs) {
        return lhs += rhs;
    }

    friend constexpr gint operator-(gint lhs, const gint &rhs) {
        return lhs -= rhs;
    }

    friend constexpr gint operator*(gint lhs, const gint &rhs) {
        return lhs *= rhs;
    }

    friend constexpr gint operator/(gint lhs, const gint &rhs) {
        return lhs /= rhs;
    }

    friend constexpr gint operator%(gint lhs, const gint &rhs) {
        return lhs %= rhs;
    }

    friend constexpr bool operator==(const gint &lhs, const gint &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend constexpr bool operator!=(const gint &lhs, const gint &rhs) {
        return !(lhs == rhs);
    }

    friend std::istream &operator>>(std::istream &is, gint &rhs) {
        is >> rhs.x >> rhs.y;
        return is;
    }

    friend std::ostream &operator<<(std::ostream &os, const gint &rhs) {
        os << rhs.x << ' ' << rhs.y;
        return os;
    }
};
} // namespace mld

#endif // MALLARD_MATH_GAUSSIAN_INTEGER_H
