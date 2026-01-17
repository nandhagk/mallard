#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, s, t;
    std::cin >> n >> m >> s >> t;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(v, std::pair<u32, u64>{u, w});
    }

    mld::static_csr g(n, e);
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
