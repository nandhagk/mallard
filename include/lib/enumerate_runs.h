#ifndef MALLARD_ENUMERATE_RUNS_H
#define MALLARD_ENUMERATE_RUNS_H 1

#include <algorithm>

#include "lib/suffix_array_query.h"
#include "lib/longest_lyndon_prefixes.h"
#include "lib/prelude.h"

namespace mld {

// Compute all runs in given string
// Complexity: $O(N \cdot (complexity of lcplen()))$ in this implementation
// (Theoretically $O(N)$ achievable)
// Reference:
// [1] H. Bannai et al., "The "Runs" Theorem,"
//     SIAM Journal on Computing, 46(5), 1501-1514, 2017.
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R> &&
             std::integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
std::vector<std::tuple<u32, u32, u32>> enumerate_runs(R &&s, u32 lo, u32 hi) {
    if (s.empty()) return {};

    mld::suffix_array_query saq(s, hi), raq(s | std::views::reverse, hi);
    auto l1 = mld::longest_lyndon_prefixes(s, saq),
         l2 = mld::longest_lyndon_prefixes(
             s | std::views::transform([lo, hi](auto &&c) { return hi - (c - lo) - 1; }), saq);

    u32 len = static_cast<u32>(std::ranges::size(s));

    std::vector<std::tuple<u32, u32, u32>> ans;
    ans.reserve(2 * len);

    for (u32 i = 0; i < len; ++i) {
        u32 j = i + l1[i], p = i - raq.lcp(len - i, len - j),
            q = j + saq.lcp(i, j);
        if (q - p >= (j - i) * 2) ans.emplace_back(j - i, p, q);

        if (l1[i] != l2[i]) {
            j = i + l2[i], p = i - raq.lcp(len - i, len - j),
            q = j + saq.lcp(i, j);
            if (q - p >= (j - i) * 2) ans.emplace_back(j - i, p, q);
        }
    }

    std::ranges::sort(ans);
    ans.erase(std::unique(ans.begin(), ans.end()), ans.end());

    return ans;
}
} // namespace mld

#endif // MALLARD_ENUMERATE_RUNS_H
