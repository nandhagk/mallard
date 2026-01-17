#ifndef MALLARD_ACTIONS_ADD_MIN_H
#define MALLARD_ACTIONS_ADD_MIN_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/min.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct add_min : base<algebra::sum<T>>::identity {
    using operand = algebra::min<T>;
    using operation = algebra::sum<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return x ? f.val() + x.val() : x.val();
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_ADD_MIN_H
