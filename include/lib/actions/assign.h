#ifndef MALLARD_ACTIONS_ASSIGN_H
#define MALLARD_ACTIONS_ASSIGN_H 1

#include "lib/actions/base.h"
#include "lib/actions/internal/concepts.h"
#include "lib/algebra/assign.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::actions {
template <algebra::internal::magma M>
struct assign {
    using operand = M;
    using operation = algebra::assign<typename M::value_type>;

    [[nodiscard]] static constexpr operand act(const operation &f,
                                               const operand &x) noexcept {
        return f.val().value_or(x.val());
    }

    [[nodiscard]] static constexpr operation pow(const operation &f, usize n) noexcept {
        return f ? operation((static_cast<M>(*(f.val())) * n).val()) : f;
    }
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_ASSIGN_H
