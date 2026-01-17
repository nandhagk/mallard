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

        g.add_edge(u, v, w);
    }

    auto [dst, prv] = mld::dijkstra(g, s);
    if (dst[t] == std::numeric_limits<u64>::max()) {
        std::cout << -1 << '\n';
        return;
    }

    std::vector<u32> pth;
    pth.reserve(n);

    for (u32 cur = t; cur != s; cur = prv[cur]) pth.push_back(cur);
    pth.push_back(s);

    std::reverse(pth.begin(), pth.end());
    u32 k = static_cast<u32>(pth.size());

    std::cout << dst[t] << ' ' << k - 1 << '\n';
    for (u32 i = 1; i < k; ++i) std::cout << pth[i - 1] << ' ' << pth[i] << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
