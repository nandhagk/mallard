#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, s, t;
    std::cin >> n >> m >> s >> t;

    mld::dynamic_graph<std::pair<u32, u64>> g(n);
    g.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        g.add_edge(v, u, w);
    }

    auto [dst, prv] = mld::dijkstra(g, t);
    if (dst[s] == std::numeric_limits<u64>::max()) {
        std::cout << -1 << '\n';
        return;
    }

    u32 k = 0;
    for (u32 cur = s; cur != t; cur = prv[cur], ++k);

    std::cout << dst[s] << ' ' << k << '\n';
    for (u32 cur = s; cur != t;) std::cout << cur << ' ' << (cur = prv[cur]) << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
