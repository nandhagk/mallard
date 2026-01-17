#include <bits/stdc++.h>

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

    std::vector<u32> a(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) ++a[u];
    }

    std::vector<u32> dp(n);
    for (u32 u : h.tor | std::views::drop(1) | std::views::reverse)
        dp[h.par[u]] += dp[u] + a[u];

    std::vector<u32> kp(n);
    for (u32 u : h.tor | std::views::drop(1))
        kp[u] = kp[h.par[u]] + dp[h.par[u]] - (dp[u] + a[u]) + (a[u] ^ 1);

    std::vector<u32> tp(n);
    for (u32 u = 0; u < n; ++u) tp[u] = dp[u] + kp[u];

    u32 ans = *std::ranges::min_element(tp);
    std::cout << ans << '\n';
    for (u32 u = 0; u < n; ++u)
        if (tp[u] == ans) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
