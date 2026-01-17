#ifndef MALLARD_ACTIONS_ADD_MIN_CNT_H
#define MALLARD_ACTIONS_ADD_MIN_CNT_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/min_cnt.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T, typename U>
struct add_min_cnt : base<algebra::sum<T>>::identity {
    using operand = algebra::min_cnt<T, U>;
    using operation = algebra::sum<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        if (!x) return x;

        auto &&[a, b] = x.val();
        return {a + f.val(), b};
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_ADD_MIN_CNT_H
