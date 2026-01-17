#ifndef MALLARD_ALGEBRA_LXOR_H
#define MALLARD_ALGEBRA_LXOR_H 1

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct lxor : base<T>, truthy<lxor<T>>, associative, commutative<lxor<T>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    [[nodiscard]] friend constexpr lxor operator+(const lxor &lhs,
                                                  const lxor &rhs) noexcept {
        return lhs.val() ^ rhs.val();
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] friend constexpr lxor operator*(const lxor &lhs, Scalar n) noexcept {
        return n % 2 ? lhs.val() : static_cast<T>(0);
    }

    [[nodiscard]] constexpr lxor operator-() const noexcept {
        return val();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_LXOR_H
