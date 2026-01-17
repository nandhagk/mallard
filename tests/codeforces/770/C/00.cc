#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(k);
    for (auto &&x : a) std::cin >> x, --x;

    std::vector<std::pair<u32, u32>> e;
    for (u32 u = 0; u < n; ++u) {
        u32 t;
        std::cin >> t;

        while (t--) {
            u32 v;
            std::cin >> v;

            e.emplace_back(--v, u);
        }
    }

    mld::static_csr g(n, e);
    auto [z, ids] = mld::scc(g);

    std::vector<u32> cnt(z);
    for (u32 u = 0; u < n; ++u) ++cnt[ids[u]];

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, {}, [&](u32 u) { return ids[u]; });

    std::vector<u32> dp(n, 1);
    for (u32 u : ord) {
        if (cnt[ids[u]] > 1) dp[u] = 0;
        for (u32 v : g[u]) dp[v] &= dp[u];
    }

    for (u32 u : a) {
        if (!dp[u]) {
            std::cout << -1 << '\n';
            return;
        }
    }

    std::ranges::fill(dp, 0);
    for (u32 u : a) dp[u] = 1;

    for (u32 u : ord | std::views::reverse)
        for (u32 v : g[u]) dp[u] |= dp[v];

    std::cout << std::reduce(dp.begin(), dp.end()) << '\n';
    for (u32 u : ord)
        if (dp[u]) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
