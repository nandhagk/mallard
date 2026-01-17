#ifndef MALLARD_LYNDON_FACTORIZATION_H
#define MALLARD_LYNDON_FACTORIZATION_H 1

#include <ranges>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr std::vector<std::pair<u32, u32>>
lyndon_factorization(R &&s) noexcept {
    u32 len = static_cast<u32>(s.size());

    std::vector<std::pair<u32, u32>> ret;
    ret.reserve(len);

    for (u32 l = 0; l < len;) {
        u32 i = l, j = i + 1;
        for (; j < len && s[i] <= s[j]; ++j) i = (s[i] == s[j] ? i + 1 : l);

        u32 m = (j - l) / (j - i);
        for (u32 t = 0; t < m; ++t) ret.emplace_back(l, j - i), l += j - i;
    }

    return ret;
}
} // namespace mld

#endif // MALLARD_LYNDON_FACTORIZATION_H
