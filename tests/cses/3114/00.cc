#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/dynamic_graph.h"
#include "lib/four_russian.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    constexpr u32 UNSET = -1;
    std::vector<u32> d(n, UNSET);

    std::queue<u32> r;
    for (u32 u = 0; u < n; ++u)
        if (a[u] == 1) r.push(u), d[u] = 0;

    while (!r.empty()) {
        u32 u = r.front();
        r.pop();

        for (u32 v : g[u])
            if (d[v] == UNSET) r.push(v), d[v] = d[u] + 1;
    }

    mld::hld_aggregate<mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>>>
        hag(g, d);
    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        std::cout << hag.fold_path(u, v).val() * 2 + hag.dist(u, v) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
