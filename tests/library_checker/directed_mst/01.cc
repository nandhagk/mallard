#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/min_arborescence.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, s;
    std::cin >> n >> m >> s;

    std::vector<std::tuple<u32, u32, u64>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(u, v, w);
    }

    auto [w, par] = *mld::min_arborescence(n, e, s);
    std::cout << w << '\n';

    par[s] = s;
    for (auto &&x : par) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
