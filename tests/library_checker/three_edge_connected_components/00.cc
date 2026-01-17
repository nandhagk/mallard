#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/tecc.h"

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
    auto uf = mld::tecc(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n);

    for (u32 u = 0; u < n; ++u) f.emplace_back(uf.find(u), u);
    mld::static_csr h(n, f);

    u32 k = 0;
    for (auto &&cc : h)
        if (!cc.empty()) ++k;

    std::cout << k << '\n';
    for (auto &&cc : h) {
        if (cc.empty()) continue;

        std::cout << cc.size() << ' ';
        for (u32 u : cc) std::cout << u << ' ';

        std::cout << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
