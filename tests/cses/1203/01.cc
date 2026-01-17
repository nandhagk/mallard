#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/dominator_tree.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e, f;
    e.reserve(m), f.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, std::pair<u32, u64>{v, w});
        f.emplace_back(v, std::pair<u32, u64>{u, w});
    }

    mld::static_csr g(n, e), h(n, f);
    auto [d1, p1] = mld::dijkstra(g, 0);
    auto [d2, p2] = mld::dijkstra(h, n - 1);

    std::vector<std::pair<u32, u32>> x;
    x.reserve(m);

    for (u32 u = 0; u < n; ++u) {
        for (auto &&[v, w] : g[u])
            if (d1[u] + w + d2[v] == d1[n - 1]) x.emplace_back(u, v);
    }

    mld::static_csr z(n, x);
    auto idom = mld::dominator_tree(z, 0);

    std::vector<u32> ans;
    ans.reserve(n);

    for (u32 cur = n - 1; cur != 0; cur = idom[cur]) ans.push_back(cur);
    std::ranges::sort(ans);

    std::cout << ans.size() + 1 << '\n';

    std::cout << 1 << ' ';
    for (u32 u : ans) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
