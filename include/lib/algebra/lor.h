#ifndef MALLARD_ALGEBRA_LOR_H
#define MALLARD_ALGEBRA_LOR_H 1

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct lor : base<T>,
             truthy<lor<T>>,
             associative,
             commutative<lor<T>>,
             idempotent<lor<T>> {
    using operand = base<T>;

    using operand::operand;
    using operand::val;

    [[nodiscard]] friend constexpr lor operator+(const lor &lhs,
                                                 const lor &rhs) noexcept {
        return lhs.val() | rhs.val();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_LOR_H
