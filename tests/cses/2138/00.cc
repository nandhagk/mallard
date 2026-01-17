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
    std::vector<std::bitset<50'048>> dp(n);
    for (u32 u : *mld::topological_sort(g) | std::views::reverse) {
        dp[u].set(u);
        for (u32 v : g[u]) dp[u] |= dp[v];
    }

    for (auto &&b : dp) std::cout << b.count() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
