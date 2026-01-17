#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<Z> a(n);
    for (auto &&x : a) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<std::pair<Z, Z>> c;
    c.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, x, y;
        std::cin >> u >> v >> x >> y;

        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.emplace_back(x, y);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::vector<std::pair<Z, Z>> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    std::vector<Z> dp(a.begin(), a.end()), tp(n), kp(n);
    for (u32 u : h.tor | std::views::drop(1) | std::views::reverse) {
        auto &&[x, y] = d[u];
        tp[u] = x * dp[u] + y * h.siz[u];
        dp[h.par[u]] += tp[u];
    }

    for (u32 u : h.tor | std::views::drop(1)) {
        auto &&[x, y] = d[u];
        kp[u] = x * (kp[h.par[u]] + dp[h.par[u]] - tp[u]) + y * (n - h.siz[u]);
    }

    for (u32 u = 0; u < n; ++u) std::cout << (kp[u] + dp[u]).val() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
