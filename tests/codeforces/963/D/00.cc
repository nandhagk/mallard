#include <bits/stdc++.h>

#include "lib/keyword_search.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);

    u32 q;
    std::cin >> q;

    std::vector<u32> occ;
    occ.reserve(s.size());

    while (q--) {
        u32 k;
        std::string m;
        std::cin >> k >> m;

        auto [l, r] = mld::keyword_search(s, sa, m);
        if (r - l < k) {
            std::cout << -1 << '\n';
            continue;
        }

        occ.clear();
        for (u32 i = l; i < r; ++i) occ.push_back(sa[i]);
        std::ranges::sort(occ);

        u32 ans = static_cast<u32>(s.size());
        for (u32 i = 0; i < r - k + 1 - l; ++i)
            ans = std::min(ans, occ[i + k - 1] - occ[i]);

        std::cout << ans + m.size() << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
