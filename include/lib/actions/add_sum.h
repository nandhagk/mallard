#ifndef MALLARD_ACTIONS_ADD_SUM_H
#define MALLARD_ACTIONS_ADD_SUM_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct add_sum : base<algebra::sum<T>>::automatic {
    using operand = algebra::sum<T>;
    using operation = algebra::sum<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return x + f;
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_ADD_SUM_H
