#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/bcc.h"
#include "lib/block_cut_tree.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    auto [cut, ccs] = mld::bcc(g);
    auto [k, ids, bct] = mld::block_cut_tree(g, cut, ccs);

    std::vector<u32> a(k);
    for (u32 u = 0; u < n; ++u)
        if (cut[u]) ++a[ids[u]];

    mld::hld_aggregate<mld::scan_table<mld::algebra::sum<u32>>> hag(bct, a);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        std::cout << hag.fold_path(ids[u], ids[v]).val() - cut[u] - cut[v] << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
