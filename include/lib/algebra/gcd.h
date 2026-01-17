#ifndef MALLARD_ALGEBRA_GCD_H
#define MALLARD_ALGEBRA_GCD_H 1

#include <limits>

#include "lib/algebra/base.h"
#include "lib/math.h"

namespace mld::algebra {
template <typename T>
struct gcd : base<T>,
             truthy<gcd<T>>,
             associative,
             commutative<gcd<T>>,
             idempotent<gcd<T>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    [[nodiscard]] friend constexpr gcd operator+(const gcd &lhs,
                                                 const gcd &rhs) noexcept {
        return mld::gcd(lhs.val(), rhs.val());
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_GCD_H
