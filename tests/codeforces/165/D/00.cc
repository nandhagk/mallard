#include <bits/stdc++.h>

#include "lib/algebra/min.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
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
    mld::hld_aggregate<mld::segment_tree<mld::algebra::min<u32>>> hag(
        g, std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 1; }));

    for (auto &&[u, v] : e)
        if (hag.par[u] == v) std::swap(u, v);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 k;
            std::cin >> k;

            auto &&[_, v] = e[(--k) << 1];
            hag.set(v, 1);
        } else if (t == 2) {

            u32 k;
            std::cin >> k;

            auto &&[_, v] = e[(--k) << 1];
            hag.set(v, 0);
        } else {
            u32 u, v;
            std::cin >> u >> v;

            if (hag.fold_path(--u, --v, 1) == 0)
                std::cout << -1 << '\n';
            else
                std::cout << hag.dist(u, v) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
