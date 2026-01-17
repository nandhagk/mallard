#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/bipolar_order.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, s, t;
    std::cin >> n >> m >> s >> t;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    if (auto x = mld::bipolar_order(g, s, t)) {
        std::cout << "Yes\n";
        for (u32 u : *x) std::cout << u << ' ';
        std::cout << '\n';
    } else {
        std::cout << "No\n";
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
