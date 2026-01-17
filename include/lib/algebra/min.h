#ifndef MALLARD_ALGEBRA_MIN_H
#define MALLARD_ALGEBRA_MIN_H 1

#include <algorithm>
#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T, T inf = std::numeric_limits<T>::max()>
struct min : base<T>,
             truthy<min<T, inf>>,
             associative,
             commutative<min<T, inf>>,
             idempotent<min<T, inf>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    constexpr min() noexcept
        : min(inf) {}

    [[nodiscard]] friend constexpr min operator+(const min &lhs,
                                                 const min &rhs) noexcept {
        return std::min(lhs.val(), rhs.val());
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_MIN_H
