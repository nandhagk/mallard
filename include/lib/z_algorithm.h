#ifndef MALLARD_Z_ALGORITHM_H
#define MALLARD_Z_ALGORITHM_H 1

#include <cassert>
#include <ranges>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr std::vector<u32> z_algorithm(R &&s) noexcept {
    u32 len = static_cast<u32>(std::ranges::size(s));
    assert(len != 0);

    std::vector<u32> z(len);
    z[0] = len;

    for (u32 i = 1, j = 0; i < len;) {
        for (; i + j < len && s[j] == s[i + j]; ++j);
        z[i] = j;
        if (j == 0) {
            ++i;
        } else {
            u32 k = 1;
            for (; k < j && k + z[k] < j; ++k) z[i + k] = z[k];
            i += k;
            j -= k;
        }
    }

    return z;
}
} // namespace mld

#endif // MALLARD_Z_ALGORITHM_H
