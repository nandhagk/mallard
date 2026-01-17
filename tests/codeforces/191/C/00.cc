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

    u32 k;
    std::cin >> k;

    std::vector<u32> ans(n);
    while (k--) {
        u32 u, v;
        std::cin >> u >> v;

        u32 w = h.lca(--u, --v);
        ++ans[u], ++ans[v], ans[w] -= 2;
    }

    for (u32 u : h.tor | std::views::drop(1) | std::views::reverse)
        ans[h.par[u]] += ans[u];

    for (auto &&[u, v] : e)
        if (h.par[v] == u) std::cout << ans[v] << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
