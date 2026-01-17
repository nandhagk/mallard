#ifndef MALLARD_ACTIONS_CHMIN_MIN_H
#define MALLARD_ACTIONS_CHMIN_MIN_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/min.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct chmin_min : base<algebra::min<T>>::automatic {
    using operand = algebra::min<T>;
    using operation = algebra::min<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return x + f;
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_CHMIN_MIN_H
