#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/dynamic_mst.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> e;
    e.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v, w);
    }

    u32 q;
    std::cin >> q;

    mld::offline::dynamic_mst<u64> dmt(n);
    dmt.reserve(m + q);

    for (auto &&[u, v, w] : e) dmt.update(u, v, w);

    auto f = [](u64 ans, u32) { std::cout << ans << '\n'; };
    while (q--) {
        u32 i, w;
        std::cin >> i >> w;

        auto &&[u, v, z] = e[--i];
        dmt.update(u, v, z = w);
        dmt.query(f);
    }

    dmt.solve();
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
