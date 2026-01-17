#include <bits/stdc++.h>
#include <cmath>
#include <iomanip>

#include "lib/algebra/sum.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_aggregate.h"
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
    mld::rooted_tree_aggregate<mld::segment_tree<mld::algebra::sum<f80>>> rag(
        g, std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    u32 q;
    std::cin >> q;

    std::cout << std::fixed;
    std::cout << std::setprecision(12);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 x, y;
            std::cin >> x >> y;

            rag.set(--x, std::log10(y));
        } else {
            u32 x, y;
            std::cin >> x >> y;

            f80 z = std::min<f80>(
                9, (rag.fold_subtree(--x) + -rag.fold_subtree(--y)).val());
            std::cout << std::pow<f80>(10, z) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
