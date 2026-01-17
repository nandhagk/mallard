#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, std::pair<u32, u64>{v, w});
        e.emplace_back(v, std::pair<u32, u64>{u, w});
    }

    mld::static_csr g(n, e);
    auto [dst, prv] = mld::dijkstra(g, n - 1);
    if (dst[0] == std::numeric_limits<u64>::max()) {
        std::cout << -1 << '\n';
        return;
    }

    for (u32 cur = 0; cur != n - 1; cur = prv[cur]) std::cout << cur + 1 << ' ';
    std::cout << n << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
