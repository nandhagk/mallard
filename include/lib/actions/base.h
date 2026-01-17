#ifndef MALLARD_ACTIONS_BASE_H
#define MALLARD_ACTIONS_BASE_H 1

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/dummy.h"

namespace mld::actions {
template <typename operation>
struct base {
    struct identity {
        [[nodiscard]] static constexpr operation pow(const operation &f,
                                                     usize) noexcept {
            return f;
        }
    };

    struct automatic {
        [[nodiscard]] static constexpr operation pow(const operation &f,
                                                     usize n) noexcept {
            return f * n;
        }
    };
};
} // namespace mld::actions

#endif // MALLARD_ACTIONS_BASE_H
