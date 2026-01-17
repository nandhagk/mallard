#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/static_modint.h"
#include "lib/topological_sort.h"

using Z = mld::modint1000000007;

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

    std::vector<Z> dp(n);
    dp[0] = 1;

    for (u32 u : ord)
        for (u32 v : g[u]) dp[v] += dp[u];

    std::cout << dp[n - 1].val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
