#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
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
    mld::rooted_tree h(g);

    std::vector<u64> dp(n), tp(n), kp(n);
    for (u32 u : h.tor | std::views::drop(1) | std::views::reverse)
        tp[u] = dp[u] + h.siz[u], dp[h.par[u]] += tp[u];

    for (u32 u : h.tor | std::views::drop(1))
        kp[u] = (kp[h.par[u]] + dp[h.par[u]] - tp[u]) + (n - h.siz[u]);

    for (u32 u = 0; u < n; ++u) std::cout << dp[u] + kp[u] << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
