#ifndef MALLARD_LONGEST_LYNDON_PREFIXES_H
#define MALLARD_LONGEST_LYNDON_PREFIXES_H 1

#include <ranges>

#include "lib/prelude.h"
#include "lib/suffix_array_query.h"

namespace mld {

// Compute the longest Lyndon prefix for each suffix s[i:N]
// (Our implementation is $O(N \cdot (complexity of lcplen()))$)
// Example:
// - `teletelepathy` -> [1,4,1,2,1,4,1,2,1,4,1,2,1]
// Reference:
// [1] H. Bannai et al., "The "Runs" Theorem,"
//     SIAM Journal on Computing, 46(5), 1501-1514, 2017.
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
std::vector<u32> longest_lyndon_prefixes(R &&s, const mld::suffix_array_query &saq) {
    u32 len = static_cast<u32>(std::ranges::size(s));

    std::vector<std::pair<u32, u32>> st;
    st.reserve(len);

    std::vector<u32> ans(len);

    st.emplace_back(len, len);
    for (u32 i = len, j = i - 1; i--; j = i - 1) {
        while (st.size() > 1) {
            u32 iv = st.back().first, jv = st.back().second;
            if (u32 l = saq.lcp(i, iv); !(iv + l < len && s[i + l] < s[iv + l])) break;
            j = jv;
            st.pop_back();
        }

        st.emplace_back(i, j);
        ans[i] = j - i + 1;
    }

    return ans;
}
} // namespace mld

#endif // MALLARD_LONGEST_LYNDON_PREFIXES_H
