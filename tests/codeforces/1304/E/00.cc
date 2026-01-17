#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 x, y, a, b, k;
        std::cin >> x >> y >> a >> b >> k;

        --a, --b, --x, --y;
        u32 u = h.dist(a, b);
        u32 v = std::min(h.dist(a, x) + h.dist(y, b), h.dist(a, y) + h.dist(x, b)) + 1;

        if ((k ^ u ^ 1) % 2 && u <= k)
            std::cout << "YES\n";
        else if ((k ^ v ^ 1) % 2 && v <= k)
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
