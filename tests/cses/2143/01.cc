#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/offline/static_reachability.h"
#include "lib/prelude.h"

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

    mld::offline::static_reachability<u128> sr(g);
    sr.reserve(q);

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        sr.query(--u, --v);
    }

    for (auto &&x : sr.solve()) std::cout << (x ? "YES\n" : "NO\n");
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
