#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_csr.h"
#include "lib/incremental_becc.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::incremental_becc ibc(n);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        ibc.merge(u, v);
    }

    mld::dynamic_csr<u32> ccs(n);
    ccs.reserve(n);

    for (u32 u = 0; u < n; ++u) ccs[ibc.find(u)].emplace_back(u);

    u32 k = 0;
    for (u32 u = 0; u < n; ++u) k += !ccs[u].empty();

    std::cout << k << '\n';
    for (u32 u = 0; u < n; ++u) {
        auto &&cc = ccs[u];
        if (cc.empty()) continue;

        std::cout << cc.size() << ' ';
        for (u32 v : cc) std::cout << v << ' ';

        std::cout << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
