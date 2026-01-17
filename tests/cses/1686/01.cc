#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/static_csr.h"
#include "lib/topological_sort.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> c(n);
    for (auto &&x : c) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = scc(g);

    std::vector<u64> d(k);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(m);

    for (u32 u = 0; u < n; ++u) {
        d[ids[u]] += c[u];
        for (u32 v : g[u])
            if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);
    }

    mld::static_csr h(k, f);
    for (u32 u : *mld::topological_sort(h) | std::views::reverse) {
        u64 x = 0;
        for (u32 v : h[u]) x = std::max(x, d[v]);
        d[u] += x;
    }

    std::cout << *std::ranges::max_element(d) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
