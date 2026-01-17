#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    auto f = [&](u32 a, u32 b, u32 c) {
        auto [u, v] = *h.path_intersection(a, c, b, c);
        return h.dist(u, v);
    };

    while (q--) {
        u32 a, b, c;
        std::cin >> a >> b >> c;

        --a, --b, --c;
        std::cout << std::max({f(a, b, c), f(a, c, b), f(b, c, a)}) + 1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
