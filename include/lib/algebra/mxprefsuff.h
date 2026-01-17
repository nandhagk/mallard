#ifndef MALLARD_ALGEBRA_MXPREFSUFF_H
#define MALLARD_ALGEBRA_MXPREFSUFF_H 1

#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct mxprefsuff : base<std::tuple<T, T, T>>,
                    truthy<mxprefsuff<T>>,
                    associative,
                    scalar_multipliable<mxprefsuff<T>>::automatic {
    using operand = base<std::tuple<T, T, T>>;

    using operand::operand;
    using operand::val;

    constexpr mxprefsuff() noexcept
        : mxprefsuff(std::numeric_limits<T>::lowest()) {}

    explicit constexpr mxprefsuff(const T &v) noexcept
        : mxprefsuff(v, v, v) {}

    [[nodiscard]] friend constexpr mxprefsuff
    operator+(const mxprefsuff &lhs, const mxprefsuff &rhs) noexcept {
        if (!lhs) return rhs;
        if (!rhs) return lhs;

        auto &&[a, b, c] = lhs.val();
        auto &&[p, q, r] = rhs.val();
        return mxprefsuff(a + p, std::max(b, a + q), std::max(c + p, r));
    }

    [[nodiscard]] friend constexpr mxprefsuff
    operator+(const mxprefsuff &lhs) noexcept {
        auto &&[a, b, c] = lhs.val();
        return {a, c, b};
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_PREFSUFF_H
