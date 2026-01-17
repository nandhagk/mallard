#ifndef MALLARD_ACTIONS_MUL_PROD_H
#define MALLARD_ACTIONS_MUL_PROD_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/prod.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct mul_prod : base<algebra::prod<T>>::automatic {
    using operand = algebra::prod<T>;
    using operation = algebra::prod<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return x + f;
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_MUL_PROD_H
