#ifndef MALLARD_ALGEBRA_ASSIGN_H
#define MALLARD_ALGEBRA_ASSIGN_H 1

#include <optional>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct assign : base<std::optional<T>>, associative, idempotent<assign<T>> {
    using operand = base<std::optional<T>>;

    using operand::operand;
    using operand::val;

    [[nodiscard]] friend constexpr assign operator+(const assign &lhs,
                                                    const assign &rhs) noexcept {
        return rhs ? rhs : lhs;
    }

    [[nodiscard]] constexpr operator bool() const noexcept {
        return val().has_value();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_ASSIGN_H
