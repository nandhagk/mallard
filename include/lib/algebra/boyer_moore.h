#ifndef MALLARD_ALGEBRA_BOYER_MOORE_H
#define MALLARD_ALGEBRA_BOYER_MOORE_H 1

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct boyer_moore : base<std::pair<T, u32>>,
                     truthy<boyer_moore<T>>,
                     associative,
                     commutative<boyer_moore<T>> {
    using size_type = u32;

    using operand = base<std::pair<T, size_type>>;

    using operand::operand;
    using operand::val;

    explicit constexpr boyer_moore(const T &v) noexcept
        : boyer_moore(v, 1) {}

    [[nodiscard]] friend constexpr boyer_moore
    operator+(const boyer_moore &lhs, const boyer_moore &rhs) noexcept {
        auto &&[a, b] = lhs.val();
        auto &&[p, q] = rhs.val();

        if (a == p) return {a, b + q};
        if (b < q) return {p, q - b};
        return {a, b - q};
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] friend constexpr boyer_moore operator*(const boyer_moore &lhs,
                                                         Scalar n) noexcept {
        auto &&[a, b] = lhs.val();
        return {a, b * n};
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_BOYER_MOORE_H
