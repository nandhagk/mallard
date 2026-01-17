#ifndef MALLARD_ACTIONS_OR_AND_H
#define MALLARD_ACTIONS_OR_AND_H 1

#include "lib/actions/base.h"
#include "lib/algebra/land.h"
#include "lib/algebra/lor.h"
#include "lib/lazy_segment_tree.h"

namespace mld::actions {
template <typename T>
struct or_and : mld::actions::base<mld::algebra::lor<T>>::automatic {
    using operand = mld::algebra::land<T>;
    using operation = mld::algebra::lor<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return f + x.val();
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_OR_AND_H
