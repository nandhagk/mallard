#ifndef MALLARD_ALGEBRA_MAX_H
#define MALLARD_ALGEBRA_MAX_H 1

#include <algorithm>
#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T, T inf = std::numeric_limits<T>::lowest()>
struct max : base<T>,
             truthy<max<T, inf>>,
             associative,
             commutative<max<T, inf>>,
             idempotent<max<T, inf>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    constexpr max() noexcept
        : max(inf) {}

    [[nodiscard]] friend constexpr max operator+(const max &lhs,
                                                 const max &rhs) noexcept {
        return std::max(lhs.val(), rhs.val());
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_MAX_H
