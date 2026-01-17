#ifndef MALLARD_ALGEBRA_LAND_H
#define MALLARD_ALGEBRA_LAND_H 1

#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct land : base<T>,
              truthy<land<T>>,
              associative,
              commutative<land<T>>,
              idempotent<land<T>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    constexpr land() noexcept
        : land(std::numeric_limits<T>::max()) {}

    [[nodiscard]] friend constexpr land operator+(const land &lhs,
                                                  const land &rhs) noexcept {
        return lhs.val() & rhs.val();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_LAND_H
