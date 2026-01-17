#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    std::vector<u32> a(n);
    for (u32 u = 0; u < n; ++u) {
        i32 p, t;
        std::cin >> p >> t;

        if (p == -1) {
            e.emplace_back(n, u);
            continue;
        }

        e.emplace_back(--p, u);
        a[u] = t;
    }

    mld::static_csr g(n + 1, e);
    mld::rooted_tree_lca h(g, n);

    std::vector<std::pair<u32, u32>> dp(n + 1);
    dp[n] = {n, n};
    for (u32 u : h.tor | std::views::drop(1)) {
        u32 p = h.par[u];
        if (a[u])
            dp[u] = {u, dp[p].second};
        else
            dp[u] = {dp[p].first, u};
    }

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t, u, v;
        std::cin >> t >> u >> v;

        --u, --v;
        u32 w = h.lca(u, v);
        if (w == n) {
            std::cout << "NO\n";
            continue;
        }

        if (t == 1) {
            std::cout << ((u != v && h.is_ancestor(u, v) &&
                           h.is_ancestor(dp[v].first, u))
                              ? "YES\n"
                              : "NO\n");
        } else {
            std::cout << ((w != v && h.is_ancestor(dp[u].first, w) &&
                           h.is_ancestor(dp[v].second, w))
                              ? "YES\n"
                              : "NO\n");
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
