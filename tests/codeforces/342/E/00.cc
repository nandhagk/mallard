#include <bits/stdc++.h>

#include "lib/ctd.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        i32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    mld::rooted_tree_lca h(g);

    constexpr u32 UNSET = -1;
    auto [_, par] = mld::ctd(g);

    std::vector<u32> best(n, n);

    best[0] = 0;
    for (u32 u = par[0]; u != UNSET; u = par[u])
        best[u] = std::min(best[u], h.dist(u, 0));

    while (m--) {
        u32 t, u;
        std::cin >> t >> u;

        --u;
        if (t == 1) {
            best[u] = 0;
            for (u32 v = par[u]; v != UNSET; v = par[v])
                best[v] = std::min(best[v], h.dist(u, v));
        } else {
            u32 ans = best[u];
            for (u32 v = par[u]; v != UNSET; v = par[v])
                ans = std::min(ans, best[v] + h.dist(u, v));

            std::cout << ans << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
