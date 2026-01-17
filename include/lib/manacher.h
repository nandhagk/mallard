#ifndef MALLARD_MANACHER_H
#define MALLARD_MANACHER_H 1

#include <ranges>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr std::pair<std::vector<u32>, std::vector<u32>>
manacher(R &&s) noexcept {
    u32 len = static_cast<u32>(std::ranges::size(s));
    std::vector<u32> odd(len), even(len);

    for (u32 q = 0, l = 0, r = 0; q < len; ++q) {
        u32 &d = odd[q];
        d = r >= q ? std::min(r - q, odd[r - q + l]) : 0;
        for (; q >= d && q + d < len && s[q - d] == s[q + d]; ++d);
        if (u32 k = q + d - 1; k > r) r = k, l = q - d + 1;
    }

    for (u32 q = 1, l = 0, r = 0; q < len; ++q) {
        u32 &d = even[q];
        d = r >= q ? std::min(r - q, even[r - q + l + 1]) : 0;
        for (; q >= d + 1 && q + d < len && s[q - d - 1] == s[q + d]; ++d);
        if (u32 k = q + d; k > r) r = k, l = q - d - 1;
    }

    return {std::move(odd), std::move(even)};
}
} // namespace mld

#endif // MALLARD_MANACHER_H
