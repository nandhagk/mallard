#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/bcc.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

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
    auto [_, ccs] = mld::bcc(g);

    std::cout << ccs.size() << '\n';
    for (auto &&cc : ccs) {
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
