#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/suffix_array.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, --x;

    if (n == 1) {
        std::cout << 1 << '\n';
        std::cout << 1 << '\n';
        for (auto &&x : a) std::cout << x + 1 << ' ';
        return;
    }

    auto sa = mld::suffix_array(a, m);
    auto lcp = mld::lcp_array(a, sa);

    mld::segment_tree<mld::algebra::min<u32>> st(lcp);

    std::vector<u64> ans;
    ans.reserve(n);

    for (u32 i = 0; i < n - 1; ++i) {
        auto f = [&](auto &&x) { return lcp[i] <= x; };

        u32 l = st.min_left(i + 1, f);
        u32 r = st.max_right(i, f);

        ans.push_back(u64(lcp[i]) * (r - l + 1));
    }

    u32 k = static_cast<u32>(std::ranges::max_element(ans) - ans.begin());
    if (ans[k] < n) {
        std::cout << n << '\n';
        std::cout << n << '\n';

        for (auto &&x : a) std::cout << x + 1 << ' ';
    } else {
        std::cout << ans[k] << '\n';
        std::cout << lcp[k] << '\n';

        for (u32 i = 0; i < lcp[k]; ++i) std::cout << a[sa[k] + i] + 1 << ' ';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
