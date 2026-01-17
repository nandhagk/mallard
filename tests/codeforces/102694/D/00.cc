#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/sparse_table.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    assert(m == n - 1);

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> c;
    c.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.emplace_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree r(g);

    std::vector<u32> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (r.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    mld::hld_aggregate<mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>>>
        hag(g, d);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        std::cout << hag.fold_path(--u, --v, 1).val() << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
