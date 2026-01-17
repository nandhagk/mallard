#ifndef MALLARD_ALGEBRA_MIN_CNT_H
#define MALLARD_ALGEBRA_MIN_CNT_H 1

#include <algorithm>
#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T, typename U>
struct min_cnt : base<std::pair<T, U>>,
                 truthy<min_cnt<T, U>>,
                 associative,
                 commutative<min_cnt<T, U>>,
                 idempotent<min_cnt<T, U>> {
    using operand = base<std::pair<T, U>>;

    using operand::operand;
    using operand::val;

    constexpr min_cnt() noexcept
        : min_cnt(std::numeric_limits<T>::max(), 0) {}

    [[nodiscard]] friend constexpr min_cnt operator+(const min_cnt &lhs,
                                                     const min_cnt &rhs) noexcept {
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

#endif // MALLARD_ALGEBRA_MIN_CNT_H
