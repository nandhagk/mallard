#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/static_csr.h"
#include "lib/rooted_tree_lca.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        std::cout << h.lca(--u, --v) + 1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
