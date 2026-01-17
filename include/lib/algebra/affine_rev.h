#ifndef MALLARD_ALGEBRA_AFFINE_REV_H
#define MALLARD_ALGEBRA_AFFINE_REV_H 1

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct affine_rev : base<std::tuple<T, T, T>>,
                    truthy<affine_rev<T>>,
                    associative,
                    scalar_multipliable<affine_rev<T>>::automatic {
    using operand = base<std::tuple<T, T, T>>;

    using operand::operand;
    using operand::val;

    constexpr affine_rev() noexcept
        : affine_rev(1, 0, 0) {}

    constexpr affine_rev(const T &a, const T &b) noexcept
        : affine_rev(a, b, b) {}

    [[nodiscard]] friend constexpr affine_rev
    operator+(const affine_rev &lhs, const affine_rev &rhs) noexcept {
        auto &&[m, c, u] = lhs.val();
        auto &&[n, _, d] = rhs.val();
        return affine_rev(m * n, rhs(c), m * d + u);
    }

    [[nodiscard]] constexpr auto operator()(const T &x) const noexcept {
        auto &&[m, c, _] = val();
        return m * x + c;
    }

    [[nodiscard]] friend constexpr affine_rev
    operator+(const affine_rev &lhs) noexcept {
        auto &&[m, c, d] = lhs.val();
        return {m, d, c};
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_AFFINE_REV_H
