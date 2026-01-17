#include <bits/stdc++.h>

#pragma GCC target("popcnt")

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/topological_sort.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    mld::static_csr g(n, e);

    auto ord = *mld::topological_sort(g);

    std::vector<u32> rnk(n);
    for (u32 u = 0; u < n; ++u) rnk[ord[u]] = u;

    std::ranges::sort(e,
                      [&](auto &&a, auto &&b) { return rnk[a.first] > rnk[b.first]; });

    std::vector<u32> ans(n);

    std::vector<u64> dp(n);
    for (u32 l = 0; l < n; l += 64) {
        u32 r = std::min(n, l + 64);

        std::ranges::fill(dp, 0);
        for (u32 u = l; u < r; ++u) dp[u] = u64{1} << (u - l);
        for (auto &&[u, v] : e) dp[u] |= dp[v];

        for (u32 u = 0; u < n; ++u) ans[u] += std::popcount(dp[u]);
    }

    for (auto &&x : ans) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
