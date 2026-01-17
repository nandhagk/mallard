#ifndef MALLARD_ALGEBRA_SUM_H
#define MALLARD_ALGEBRA_SUM_H 1

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct sum : base<T>, truthy<sum<T>>, associative, commutative<sum<T>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    [[nodiscard]] friend constexpr sum operator+(const sum &lhs,
                                                 const sum &rhs) noexcept {
        return lhs.val() + rhs.val();
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] friend constexpr sum operator*(const sum &lhs, Scalar n) noexcept {
        return lhs.val() * n;
    }

    [[nodiscard]] constexpr sum operator-() const noexcept {
        return -val();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_SUM_H
