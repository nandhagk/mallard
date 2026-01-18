#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/chromatic_partition.h"
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
    auto [k, _] = mld::chromatic_partition(g);

    std::cout << k << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
