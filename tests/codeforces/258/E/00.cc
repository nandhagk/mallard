#include <bits/stdc++.h>

#include "lib/actions/add_min_cnt.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_aggregate.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

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
    mld::rooted_tree_aggregate<
        mld::lazy_segment_tree<mld::actions::add_min_cnt<u32, u32>>>
        rag(g, std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                   return std::pair<u32, u32>{0, 1};
               }));

    std::vector<std::pair<u32, u32>> f;
    f.reserve(2 * q);

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        f.emplace_back(u, v);
        f.emplace_back(v, u);
    }

    mld::static_csr z(n, f);

    std::vector<u32> ans(n);
    auto dfs = [&](auto &&self, u32 u, u32 t) -> void {
        for (u32 v : z[u]) rag.apply_subtree(v, 1);
        if (!z[u].empty()) rag.apply_subtree(u, 1);

        auto [x, y] = rag.fold().val();
        if (x != 0)
            ans[u] = n - 1;
        else
            ans[u] = std::max<u32>(1, n - y) - 1;

        for (u32 v : g[u])
            if (v != t) self(self, v, u);

        if (!z[u].empty()) rag.apply_subtree(u, -1);
        for (u32 v : z[u]) rag.apply_subtree(v, -1);
    };

    dfs(dfs, 0, 0);
    for (auto &&x : ans) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
