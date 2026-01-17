#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/becc.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    auto [_, ids] = mld::becc(g);

    u32 k = 0;
    for (u32 u = 0; u < n; ++u)
        for (u32 v : g[u]) k += u < v && ids[u] != ids[v];

    std::cout << k << '\n';
    for (u32 u = 0; u < n; ++u) {
        for (u32 v : g[u])
            if (u < v && ids[u] != ids[v]) std::cout << u + 1 << ' ' << v + 1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
