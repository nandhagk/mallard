#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/min_arborescence.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, s;
    std::cin >> n >> m >> s;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(u, std::pair<u32, u64>{v, w});
    }

    mld::static_csr g(n, e);

    auto [w, par] = *mld::min_arborescence(g, s, e.size());
    std::cout << w << '\n';

    par[s] = s;
    for (auto &&x : par) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
