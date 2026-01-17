#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        --a, --b;
        e.emplace_back(a, b);
        e.emplace_back(b, a);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    std::vector<u32> ans(n);
    while (m--) {
        u32 a, b;
        std::cin >> a >> b;

        u32 c = h.lca(--a, --b);
        --ans[c], ++ans[a], ++ans[b];
        if (c != 0) --ans[h.par[c]];
    }

    for (u32 u : h.tor | std::views::drop(1) | std::views::reverse)
        ans[h.par[u]] += ans[u];
    for (auto &&x : ans) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
