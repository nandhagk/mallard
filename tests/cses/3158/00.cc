#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> c(m);
    for (auto &&x : c) std::cin >> x;

    mld::dynamic_graph<std::pair<u32, u64>> g(n + m);
    g.reserve(400'005);

    for (u32 i = 0; i < m; ++i) {
        u32 k;
        std::cin >> k;

        while (k--) {
            u32 u;
            std::cin >> u;

            --u;
            g.add_edge(u, i + n, c[i]);
            g.add_edge(i + n, u, c[i]);
        }
    }

    auto [dst, _] = dijkstra(g, 0);

    for (auto &&x : dst | std::views::take(n)) std::cout << x / 2 << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
