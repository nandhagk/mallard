#ifndef MALLARD_ACTIONS_CHMAX_MAX_H
#define MALLARD_ACTIONS_CHMAX_MAX_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/max.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct chmax_max : base<algebra::max<T>>::automatic {
    using operand = algebra::max<T>;
    using operation = algebra::max<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return x + f;
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_CHMAX_MAX_H
