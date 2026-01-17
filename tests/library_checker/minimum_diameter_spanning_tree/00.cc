#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/tree_diameter.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e, f;
    e.reserve(2 * m), f.reserve(2 * n);

    std::map<std::tuple<u32, u32, u32>, u32> edg;

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(u, std::pair<u32, u64>{v, w});
        e.emplace_back(v, std::pair<u32, u64>{u, w});

        auto [a, b] = std::minmax(u, v);
        edg.emplace(std::tuple<u32, u32, u32>{a, b, w}, i);
    }

    mld::static_csr g(n, e);

    std::vector<std::pair<u64, u32>> ord;
    ord.reserve(n);

    for (u32 u = 0; u < n; ++u) {
        auto [dst, prv] = mld::dijkstra(g, u);

        f.clear();
        for (u32 v = 0; v < n; ++v) {
            if (u == v) continue;

            u32 p = prv[v];
            f.emplace_back(p, std::pair<u32, u64>{v, dst[v] - dst[p]});
            f.emplace_back(v, std::pair<u32, u64>{p, dst[v] - dst[p]});
        }

        mld::static_csr h(n, f);
        auto [_, __, dis] = mld::tree_diameter(h);

        ord.emplace_back(*std::ranges::max_element(dis), u);
    }

    auto [dim, s] = *std::ranges::min_element(ord);
    auto [dst, prv] = mld::dijkstra(g, s);

    std::cout << dim << '\n';
    for (u32 u = 0; u < n; ++u) {
        if (u == s) continue;

        auto [a, b] = std::minmax(u, prv[u]);
        std::cout << edg.find({a, b, dst[u] - dst[prv[u]]})->second << ' ';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
