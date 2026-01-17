#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(3 * m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(2 * u, std::pair<u32, u64>{2 * v, w});
        e.emplace_back(2 * u, std::pair<u32, u64>{2 * v + 1, w / 2});
        e.emplace_back(2 * u + 1, std::pair<u32, u64>{2 * v + 1, w});
    }

    mld::static_csr g(2 * n, e);
    auto [dst, _] = mld::dijkstra(g, 0);
    std::cout << dst[2 * n - 1] << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
