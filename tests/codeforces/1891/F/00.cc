#include <bits/stdc++.h>

#include "lib/actions/add_sum.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_aggregate.h"
#include "lib/static_csr.h"

void solve() {
    u32 q;
    std::cin >> q;

    std::vector<std::tuple<u32, u32, i32>> que;
    que.reserve(q);

    u32 n = 1;
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 u;
            std::cin >> u;

            que.emplace_back(t, --u, n++);
        } else {
            u32 u;
            i32 x;
            std::cin >> u >> x;

            que.emplace_back(t, --u, x);
        }
    }

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (auto &&[t, u, v] : que)
        if (t == 1) e.emplace_back(u, v);

    mld::static_csr g(n, e);
    mld::rooted_tree_aggregate<mld::dual_fenwick_tree<mld::actions::add_sum<i64>>> rag(
        g, std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    for (auto &&[t, u, x] : que)
        if (t == 1)
            rag.apply_subtree(x, -rag.get(u));
        else
            rag.apply_subtree(u, x);

    for (u32 u = 0; u < n; ++u) std::cout << rag.get(u).val() << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
