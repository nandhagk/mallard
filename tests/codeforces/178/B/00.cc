#include <bits/stdc++.h>

#include "lib/becc.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
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
    auto [k, ids] = mld::becc(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(2 * n);

    for (u32 u = 0; u < n; ++u) {
        for (u32 v : g[u])
            if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);
    }

    mld::static_csr z(k, f);
    mld::rooted_tree_lca h(z);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        std::cout << h.dist(ids[--u], ids[--v]) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
