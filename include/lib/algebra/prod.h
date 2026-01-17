#ifndef MALLARD_ALGEBRA_PROD_H
#define MALLARD_ALGEBRA_PROD_H 1

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct prod : base<T>,
              truthy<prod<T>>,
              associative,
              commutative<prod<T>>,
              scalar_multipliable<prod<T>>::automatic {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    constexpr prod() noexcept
        : prod(static_cast<T>(1)) {}

    [[nodiscard]] friend constexpr prod operator+(const prod &lhs,
                                                  const prod &rhs) noexcept {
        return lhs.val() * rhs.val();
    }

    [[nodiscard]] constexpr prod operator-() const noexcept {
        return static_cast<T>(1) / val();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_PROD_H
