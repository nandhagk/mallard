#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/topological_sort.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> c(n);
    for (auto &&x : c) std::cin >> x;

    mld::dynamic_graph<u32> g(n);
    g.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        g.add_edge(--u, --v);
    }

    auto [k, ids] = scc(g);

    std::vector<u64> d(k);

    mld::dynamic_graph<u32> h(k);
    h.reserve(m);

    for (u32 u = 0; u < n; ++u) {
        d[ids[u]] += c[u];
        for (u32 v : g[u])
            if (ids[u] != ids[v]) h.add_edge(ids[u], ids[v]);
    }

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
