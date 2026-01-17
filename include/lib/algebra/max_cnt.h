#ifndef MALLARD_ALGEBRA_MAX_CNT_H
#define MALLARD_ALGEBRA_MAX_CNT_H 1

#include <algorithm>
#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T, typename U>
struct max_cnt : base<std::pair<T, U>>,
                 truthy<max_cnt<T, U>>,
                 associative,
                 commutative<max_cnt<T, U>>,
                 idempotent<max_cnt<T, U>> {
    using operand = base<std::pair<T, U>>;

    using operand::operand;
    using operand::val;

    constexpr max_cnt() noexcept
        : max_cnt(std::numeric_limits<T>::lowest(), 0) {}

    [[nodiscard]] friend constexpr max_cnt operator+(const max_cnt &lhs,
                                                     const max_cnt &rhs) noexcept {
        if (!lhs) return rhs;
        if (!rhs) return lhs;

        auto &&[a, b] = lhs.val();
        auto &&[c, d] = rhs.val();

        if (a < c)
            return lhs;
        else if (c < a)
            return rhs;
        else
            return {a, b + d};
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_MAX_CNT_H
