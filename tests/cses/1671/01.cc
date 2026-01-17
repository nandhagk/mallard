#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, std::pair<u32, u64>{--v, w});
    }

    mld::static_csr g(n, e);

    auto [dst, _] = mld::dijkstra(g, 0);
    for (auto &&x : dst) std::cout << x << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
