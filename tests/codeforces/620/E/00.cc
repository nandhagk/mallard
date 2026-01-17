#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/assign.h"
#include "lib/algebra/lor.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_aggregate.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> c(n);
    for (auto &&x : c) std::cin >> x, --x;

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_aggregate<
        mld::lazy_segment_tree<mld::actions::assign<mld::algebra::lor<u64>>>>
        rag(g, c | std::views::transform([](u32 x) { return u64(1) << x; }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 u, x;
            std::cin >> u >> x;

            rag.apply_subtree(--u, u64(1) << (--x));
        } else {
            u32 u;
            std::cin >> u;

            std::cout << std::popcount(rag.fold_subtree(--u).val()) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
