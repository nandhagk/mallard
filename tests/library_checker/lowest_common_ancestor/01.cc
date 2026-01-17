#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/hld.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::dynamic_graph<u32> g(n);
    g.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;
        g.add_edge(p, u);
    }

    mld::hld h(g);
    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        std::cout << h.lca(u, v) << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
