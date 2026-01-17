#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::dynamic_graph<std::pair<u32, u64>> g(n);
    g.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        g.add_edge(--u, --v, w);
    }

    auto [dst, _] = mld::dijkstra(g, 0);
    for (auto &&x : dst) std::cout << x << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
