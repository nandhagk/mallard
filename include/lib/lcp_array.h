#ifndef MALLARD_LCP_ARRAY_H
#define MALLARD_LCP_ARRAY_H 1

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

#include "lib/prelude.h"

namespace mld {
// Reference:
// T. Kasai, G. Lee, H. Arimura, S. Arikawa, and K. Park,
// Linear-Time Longest-Common-Prefix Computation in Suffix Arrays and Its
// Applications
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr std::vector<u32>
lcp_array(R &&s, const std::vector<u32> &sa, const std::vector<u32> &rnk) noexcept {
    u32 len = static_cast<u32>(std::ranges::size(s));
    std::vector<u32> lcp(len - 1);

    for (u32 i = 0, h = 0; i < len; ++i) {
        if (h > 0) --h;
        if (rnk[i] == 0) continue;
        for (u32 j = sa[rnk[i] - 1]; j + h < len && i + h < len && s[j + h] == s[i + h];
             ++h);
        lcp[rnk[i] - 1] = h;
    }

    return lcp;
}

template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr std::vector<u32>
lcp_array(R &&s, const std::vector<u32> &sa) noexcept {
    assert(s.size() == sa.size());

    u32 len = static_cast<u32>(std::ranges::size(s));
    assert(len >= 1 && *std::ranges::max_element(sa) < len);

    std::vector<u32> rnk(len);
    for (u32 i = 0; i < len; ++i) rnk[sa[i]] = i;

    return lcp_array(s, sa, rnk);
}
} // namespace mld

#endif // MALLARD_LCP_ARRAY_H
