#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, std::pair<u32, u64>{--v, w});
    }

    mld::static_csr g(n, e);
    auto [dst, prv] = mld::dijkstra(g, 0);

    for (bool f = 0; q--;) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 v;
            std::cin >> v;

            --v;

            if (f) {
                g.build(n, e);
                std::tie(dst, prv) = mld::dijkstra(g, 0);
                f = 0;
            }

            std::cout << i64(dst[v] == std::numeric_limits<u64>::max() ? -1 : dst[v])
                      << '\n';
        } else {
            u32 c;
            std::cin >> c;

            while (c--) {
                u32 l;
                std::cin >> l;

                ++e[--l].second.second;
            }

            f = 1;
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
