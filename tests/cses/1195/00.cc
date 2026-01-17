#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::dynamic_graph<std::pair<u32, u64>> g(2 * n);
    g.reserve(3 * m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        g.add_edge(2 * u, 2 * v, w);
        g.add_edge(2 * u, 2 * v + 1, w / 2);
        g.add_edge(2 * u + 1, 2 * v + 1, w);
    }

    auto [dst, _] = mld::dijkstra(g, 0);
    std::cout << dst[2 * n - 1] << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
