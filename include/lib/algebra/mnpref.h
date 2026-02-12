#ifndef MALLARD_ALGEBRA_MNPREF_H
#define MALLARD_ALGEBRA_MNPREF_H 1

#include <limits>

#include "lib/algebra/base.h"

namespace mld::algebra {
template <typename T>
struct mnpref : base<std::pair<T, T>>,
                truthy<mnpref<T>>,
                associative,
                scalar_multipliable<mnpref<T>>::automatic {
    using operand = base<std::pair<T, T>>;

    using operand::operand;
    using operand::val;

    constexpr mnpref() noexcept
        : mnpref(std::numeric_limits<T>::max()) {}

    explicit constexpr mnpref(const T &v) noexcept
        : mnpref(v, v) {}

    [[nodiscard]] friend constexpr mnpref operator+(const mnpref &lhs,
                                                    const mnpref &rhs) noexcept {
        if (!lhs) return rhs;
        if (!rhs) return lhs;

        auto &&[a, b] = lhs.val();
        auto &&[p, q] = rhs.val();
        return mnpref(a + p, std::min(b, a + q));
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_MNPREF_H
