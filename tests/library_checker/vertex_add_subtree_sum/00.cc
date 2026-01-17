#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/dynamic_graph.h"
#include "lib/fenwick_tree.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> z(n);
    for (auto &&x : z) std::cin >> x;

    mld::dynamic_graph<u32> g(n);
    g.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;
        g.add_edge(p, u);
    }

    mld::hld_aggregate<mld::fenwick_tree<mld::algebra::sum<u64>>> hag(g, z);
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            hag.add(p, x);
        } else {
            u32 u;
            std::cin >> u;

            std::cout << hag.fold_subtree(u).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
