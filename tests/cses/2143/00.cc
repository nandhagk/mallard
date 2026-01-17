#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/topological_sort.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    mld::dynamic_graph<u32> g(n);
    g.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;
        g.add_edge(--u, --v);
    }

    auto [k, ids] = scc(g);

    mld::dynamic_graph<u32> h(k);
    h.reserve(m);

    for (u32 u = 0; u < n; ++u) {
        for (u32 v : g[u])
            if (ids[u] != ids[v]) h.add_edge(ids[u], ids[v]);
    }

    std::vector<std::bitset<50'000>> dp(k);
    for (u32 u : *mld::topological_sort(h) | std::views::reverse) {
        dp[u].set(u);
        for (u32 v : h[u]) dp[u] |= dp[v];
    }

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        std::cout << (dp[ids[--u]].test(ids[--v]) ? "YES\n" : "NO\n");
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
