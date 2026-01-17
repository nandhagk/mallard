#ifndef MALLARD_ALGEBRA_MXPREF_H
#define MALLARD_ALGEBRA_MXPREF_H 1

#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct mxpref : base<std::pair<T, T>>,
                truthy<mxpref<T>>,
                associative,
                scalar_multipliable<mxpref<T>>::automatic {
    using operand = base<std::pair<T, T>>;

    using operand::operand;
    using operand::val;

    constexpr mxpref() noexcept
        : mxpref(std::numeric_limits<T>::lowest()) {}

    explicit constexpr mxpref(const T &v) noexcept
        : mxpref(v, v) {}

    [[nodiscard]] friend constexpr mxpref operator+(const mxpref &lhs,
                                                    const mxpref &rhs) noexcept {
        if (!lhs) return rhs;
        if (!rhs) return lhs;

        auto &&[a, b] = lhs.val();
        auto &&[p, q] = rhs.val();
        return mxpref(a + p, std::max(b, a + q));
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_MXPREF_H
