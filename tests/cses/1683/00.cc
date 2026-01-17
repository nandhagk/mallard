#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/scc.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = scc(g);

    std::cout << k << '\n';
    for (u32 u = 0; u < n; ++u) std::cout << ids[u] + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
