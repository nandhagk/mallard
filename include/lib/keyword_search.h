#ifndef MALLARD_KEYWORD_SEARCH_H
#define MALLARD_KEYWORD_SEARCH_H 1

#include <algorithm>
#include <cassert>
#include <vector>

#include "lib/prelude.h"

namespace mld {
// Keyword search in str
// Complexity: O(min(|str|, |keyword|) * lg |str|)
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R> &&
             std::integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
[[nodiscard]] constexpr std::pair<u32, u32>
keyword_search(R &&s, const std::vector<u32> &sa, R &&t) noexcept {
    auto [l, r] = std::ranges::equal_range(sa, 0, {}, [&](u32 i) {
        return std::bit_cast<char>(std::lexicographical_compare_three_way(
            std::ranges::begin(s) + i,
            std::ranges::begin(s) +
                std::min(i + std::ranges::size(t), std::ranges::size(s)),
            std::ranges::begin(t), std::ranges::end(t)));
    });
    return {l - sa.begin(), r - sa.begin()};
}
} // namespace mld

#endif // MALLARD_KEYWORD_SEARCH_H
