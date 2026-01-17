#ifndef MALLARD_ALGEBRA_AFFINE_H
#define MALLARD_ALGEBRA_AFFINE_H 1

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct affine : base<std::pair<T, T>>,
                truthy<affine<T>>,
                associative,
                scalar_multipliable<affine<T>>::automatic {
    using operand = base<std::pair<T, T>>;

    using operand::operand;
    using operand::val;

    constexpr affine() noexcept
        : affine(1, 0) {}

    [[nodiscard]] friend constexpr affine operator+(const affine &lhs,
                                                    const affine &rhs) noexcept {
        auto &&[m, c] = lhs.val();
        auto &&[n, _] = rhs.val();
        return {m * n, rhs(c)};
    }

    [[nodiscard]] constexpr auto operator()(const T &x) const noexcept {
        auto &&[m, c] = val();
        return m * x + c;
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_AFFINE_H
