#ifndef MALLARD_ALGEBRA_MXSS_H
#define MALLARD_ALGEBRA_MXSS_H 1

#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct mxss : base<std::tuple<T, T, T, T>>,
              truthy<mxss<T>>,
              associative,
              scalar_multipliable<mxss<T>>::automatic {
    using operand = base<std::tuple<T, T, T, T>>;

    using operand::operand;
    using operand::val;

    constexpr mxss() noexcept
        : mxss(std::numeric_limits<T>::lowest()) {}

    explicit constexpr mxss(const T &v) noexcept
        : mxss(v, v, v, v) {}

    [[nodiscard]] friend constexpr mxss operator+(const mxss &lhs,
                                                  const mxss &rhs) noexcept {
        if (!lhs) return rhs;
        if (!rhs) return lhs;

        auto &&[a, b, c, d] = lhs.val();
        auto &&[p, q, r, s] = rhs.val();
        return mxss(a + p, std::max(b, a + q), std::max(c + p, r),
                    std::max({d, s, c + q}));
    }

    [[nodiscard]] friend constexpr mxss operator+(const mxss &lhs) noexcept {
        auto &&[a, b, c, d] = lhs.val();
        return {a, c, b, d};
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_MXSS_H
