#include <bits/stdc++.h>

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

    std::vector<i32> dp(n, -1), prv(n, -1);

    dp[0] = 1;
    for (u32 u : ord) {
        if (dp[u] == -1) continue;
        for (u32 v : g[u])
            if (dp[u] + 1 > dp[v]) prv[v] = u, dp[v] = dp[u] + 1;
    }

    if (dp[n - 1] == -1) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    std::vector<u32> pth;
    pth.reserve(n);

    for (u32 cur = n - 1; cur != 0; cur = prv[cur]) pth.push_back(cur);
    pth.push_back(0);

    std::cout << dp[n - 1] << '\n';
    for (u32 u : pth | std::views::reverse) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
