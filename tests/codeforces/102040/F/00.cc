#include <bits/stdc++.h>

#include "blazingio.min.h"
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

    std::vector<std::pair<u32, u32>> z;
    z.reserve(50);

    while (q--) {
        u32 k;
        std::cin >> k;

        z.clear();
        for (u32 i = 0; i < k; ++i) {
            u32 u, v;
            std::cin >> u >> v;

            z.emplace_back(--u, --v);
        }

        auto [u, v] = z[0];
        for (auto &&[a, b] : z | std::views::drop(1))
            if (auto res = h.path_intersection(u, v, a, b))
                std::tie(u, v) = *res;
            else
                goto empty;

        std::cout << h.dist(u, v) + 1 << '\n';
        continue;

    empty:;
        std::cout << 0 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    for (u32 i = 0; i < t; ++i) {
        std::cout << "Case " << i + 1 << ":\n";
        solve();
    }
}
