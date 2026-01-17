#include <bits/stdc++.h>

#include "lib/dijkstra.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, std::pair<u32, u64>{v, w});
        e.emplace_back(v, std::pair<u32, u64>{u, w});
    }

    std::vector<std::pair<u32, u32>> z(k);
    for (auto &&[u, v] : z) std::cin >> u >> v, --u, --v;

    mld::static_csr g(n, e);
    mld::static_csr<u64> d(std::views::iota(u32{0}, n) |
                           std::views::transform([&](auto) { return n; }));

    for (u32 u = 0; u < n; ++u) {
        auto [dst, _] = mld::dijkstra(g, u);
        std::ranges::copy(dst, d[u].begin());
    }

    u64 ans = std::numeric_limits<u64>::max();
    for (u32 u = 0; u < n; ++u) {
        for (auto &&[v, _] : g[u]) {
            ans = std::min(
                ans, std::transform_reduce(
                         z.begin(), z.end(), u64{0}, std::plus<>{}, [&](auto &&p) {
                             auto &&[a, b] = p;
                             return std::min(
                                 {d[a][b], d[a][u] + d[v][b], d[a][v] + d[u][b]});
                         }));
        }
    }

    std::cout << ans << std::endl;
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
