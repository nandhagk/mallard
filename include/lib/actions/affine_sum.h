#ifndef MALLARD_ACTIONS_AFFINE_SUM_H
#define MALLARD_ACTIONS_AFFINE_SUM_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/affine.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"

namespace mld::actions {
template <typename T>
struct affine_sum {
    using operand = algebra::sum<T>;
    using operation = algebra::affine<T>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return f(x.val());
    }

    [[nodiscard]] static constexpr operation pow(const operation &f,
                                                 usize len) noexcept {
        auto &&[m, c] = f.val();
        return {m, c * len};
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_AFFINE_SUM_H
