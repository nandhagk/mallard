#ifndef MALLARD_ACTIONS_OR_OR_H
#define MALLARD_ACTIONS_OR_OR_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/lor.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct or_or : base<algebra::lor<T>>::automatic {
    using operand = algebra::lor<T>;
    using operation = algebra::lor<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return x + f;
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_OR_OR_H
