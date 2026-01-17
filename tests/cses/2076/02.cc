#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/becc.h"
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

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    auto [_, ids] = mld::becc(g);

    auto k = std::ranges::count_if(e, [&](auto &&p) {
        return p.first < p.second && ids[p.first] != ids[p.second];
    });
    std::cout << k << '\n';

    for (auto &&[u, v] : e)
        if (u < v && ids[u] != ids[v]) std::cout << u + 1 << ' ' << v + 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
