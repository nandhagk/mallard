#ifndef MALLARD_KMP_H
#define MALLARD_KMP_H

#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr std::vector<u32> kmp(R &&s) noexcept {
    u32 len = static_cast<u32>(s.size());

    std::vector<u32> k(len);
    for (u32 i = 1; i < len; ++i) {
        u32 j = k[i - 1];
        while (j > 0 && s[i] != s[j]) j = k[j - 1];

        if (s[i] == s[j]) ++j;
        k[i] = j;
    }

    return k;
}
} // namespace mld

#endif // MALLARD_KMP_H
