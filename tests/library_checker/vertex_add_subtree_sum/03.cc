#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/static_csr.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_aggregate.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> z(n);
    for (auto &&x : z) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_aggregate<mld::fenwick_tree<mld::algebra::sum<u64>>> rag(g, z);
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            rag.add(p, x);
        } else {
            u32 u;
            std::cin >> u;

            std::cout << rag.fold_subtree(u).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
