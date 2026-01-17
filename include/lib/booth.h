#ifndef MALLARD_BOOTH_H
#define MALLARD_BOOTH_H 1

#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr u32 booth(R &&s) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(s.size());

    auto pos = [len](u32 i) { return i >= len ? i - len : i; };
    std::vector<u32> f(2 * len, UNSET);

    u32 k = 0;
    for (u32 j = 1; j < 2 * len; ++j) {
        u32 i = f[j - k - 1];
        for (; i != UNSET && s[pos(j)] != s[pos(k + i + 1)]; i = f[i])
            if (s[pos(j)] < s[pos(k + i + 1)]) k = j - i - 1;

        if (i == UNSET && s[pos(j)] != s[pos(k + i + 1)]) {
            if (s[pos(j)] < s[pos(k + i + 1)]) k = j;
            f[j - k] = UNSET;
        } else {
            f[j - k] = i + 1;
        }
    }

    return k;
}
} // namespace mld

#endif // MALLARD_BOOTH_H
