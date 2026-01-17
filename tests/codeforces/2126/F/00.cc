#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/rooted_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> c;
    c.reserve(n);
    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.push_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::vector<u32> d(n);

    u64 ans = 0;
    std::vector<std::map<u32, u64>> dp(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        dp[u][a[v]] += d[v] = c[i];
        if (a[u] != a[v]) ans += d[v];
    }

    while (q--) {
        u32 u, x;
        std::cin >> u >> x;

        --u;
        if (u != 0) {
            u32 p = h.par[u];
            dp[p][a[u]] -= d[u];
            if (a[p] != a[u]) ans -= d[u];
        }

        ans += dp[u][a[u]];
        a[u] = x;
        ans -= dp[u][x];

        if (u != 0) {
            u32 p = h.par[u];
            dp[p][a[u]] += d[u];
            if (a[p] != a[u]) ans += d[u];
        }

        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
