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

    if (k != 1) {
        std::cout << "NO\n";
        auto [a, b] = std::ranges::minmax_element(ids);
        std::cout << b - ids.begin() + 1 << ' ' << a - ids.begin() + 1 << '\n';
    } else {
        std::cout << "YES\n";
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
