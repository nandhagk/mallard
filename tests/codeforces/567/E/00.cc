#include <bits/stdc++.h>
#include <limits>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/incremental_becc.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, s, t;
    std::cin >> n >> m >> s >> t;

    --s, --t;
    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, std::pair<u32, u64>{--v, w});
    }

    mld::static_csr g(n, e);
    auto [ds, ps] = mld::dijkstra(g, s);

    for (auto &&[u, p] : e) std::swap(u, p.first);

    mld::static_csr h(n, e);
    auto [dt, pt] = mld::dijkstra(h, t);

    mld::incremental_becc ibc(n);
    for (u32 u = 0; u < n; ++u) {
        if (ds[u] == std::numeric_limits<u64>::max()) continue;
        for (auto &&[v, w] : g[u])
            if (dt[v] != std::numeric_limits<u64>::max() && ds[u] + w + dt[v] == ds[t])
                ibc.merge(u, v);
    }

    for (auto &&[v, p] : e) {
        auto &&[u, w] = p;
        if (std::max(ds[u], dt[v]) == std::numeric_limits<u64>::max()) {
            std::cout << "NO\n";
            continue;
        }

        if (u64 z = ds[u] + w + dt[v] - ds[t]; z == 0 && !ibc.is_same(u, v))
            std::cout << "YES\n";
        else if (w > z + 1)
            std::cout << "CAN " << z + 1 << '\n';
        else
            std::cout << "NO\n";
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
