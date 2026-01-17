#ifndef MALLARD_ACTIONS_ADD_MAX_H
#define MALLARD_ACTIONS_ADD_MAX_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/max.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct add_max : base<algebra::sum<T>>::identity {
    using operand = algebra::max<T>;
    using operation = algebra::sum<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return x ? f.val() + x.val() : x.val();
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_ADD_MAX_H
