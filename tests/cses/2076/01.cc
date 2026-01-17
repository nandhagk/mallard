#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/incremental_becc.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    mld::incremental_becc ibc(n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        ibc.merge(u, v);
    }

    auto k = std::ranges::count_if(
        e, [&](auto &&p) { return !ibc.is_same(p.first, p.second); });
    std::cout << k << '\n';

    for (auto &&[u, v] : e)
        if (!ibc.is_same(u, v)) std::cout << u + 1 << ' ' << v + 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
