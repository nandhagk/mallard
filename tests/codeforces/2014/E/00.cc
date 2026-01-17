#include <bits/stdc++.h>
#include <limits>

#include "lib/dijkstra.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, h;
    std::cin >> n >> m >> h;

    std::vector<u32> a(h);
    for (auto &&x : a) std::cin >> x, --x;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(4 * m + n);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u << 1 | 0, std::pair<u32, u64>{v << 1 | 0, w});
        e.emplace_back(v << 1 | 0, std::pair<u32, u64>{u << 1 | 0, w});

        e.emplace_back(u << 1 | 1, std::pair<u32, u64>{v << 1 | 1, w / 2});
        e.emplace_back(v << 1 | 1, std::pair<u32, u64>{u << 1 | 1, w / 2});
    }

    for (u32 u : a) e.emplace_back(u << 1 | 0, std::pair<u32, u64>{u << 1 | 1, 0});

    mld::static_csr g(2 * n, e);

    u32 s = 0, t = n - 1;
    auto [ds, ps] = mld::dijkstra(g, s << 1 | 0);
    auto [dt, pt] = mld::dijkstra(g, t << 1 | 0);

    if (ds[t << 1 | 0] == std::numeric_limits<u64>::max()) {
        std::cout << -1 << '\n';
        return;
    }

    u64 ans = std::numeric_limits<u64>::max();
    for (u32 u = 0; u < n; ++u) {
        ans = std::min(ans, std::max(std::min(ds[u << 1 | 0], ds[u << 1 | 1]),
                                     std::min(dt[u << 1 | 0], dt[u << 1 | 1])));
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
