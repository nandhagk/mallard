#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/assign.h"
#include "lib/algebra/sum.h"
#include "lib/lazy_segment_tree.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());
    if (n == 1) {
        std::cout << 1 << '\n';
        return;
    }

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);
    auto lcp = mld::lcp_array(s, sa);

    mld::lazy_segment_tree<mld::actions::assign<mld::algebra::sum<u64>>> st(lcp);

    u64 ans = 0;
    for (u32 i = n - 1; i--;) {
        u32 lo = i, hi = n - 1;
        while (hi - lo > 1) {
            u32 m = std::midpoint(lo, hi);
            if (st.get(m) >= lcp[i])
                lo = m;
            else
                hi = m;
        }

        st.apply(i, hi, lcp[i]);
        ans += st.fold(i, n - 1).val();
    }

    std::cout << ans + u64(n) * (n + 1) / 2 << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
