#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ccc.h"
#include "lib/dynamic_csr.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);

    auto [k, ids] = ccc(g);
    std::cout << k << '\n';

    mld::dynamic_csr<u32> ccs(k);
    ccs.reserve(n);

    for (u32 u = 0; u < n; ++u) ccs[ids[u]].emplace_back(u);

    for (u32 i = 0; i < k; ++i) {
        auto &&cc = ccs[i];

        std::cout << cc.size() << ' ';
        for (u32 u : cc) std::cout << u << ' ';

        std::cout << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
