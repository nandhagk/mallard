#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);

    std::vector<u32> d(n);
    for (u32 u = 0; u < n; ++u) d[u] = static_cast<u32>(g[u].size());

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n);

    std::queue<u32> q;
    for (u32 u = 0; u < n; ++u)
        if (a[u] == d[u]) q.push(u);

    while (!q.empty()) {
        u32 u = q.front();
        q.pop();

        d[u] = n;
        for (u32 v : g[u]) {
            if (d[v] == n) continue;

            f.emplace_back(u, v);
            if (a[v] == --d[v]) q.push(v);
        }
    }

    std::ranges::reverse(f);

    u64 ans = 0;

    std::vector<u64> dp(n);
    for (u32 l = 0; l < n; l += 64) {
        u32 r = std::min(n, l + 64);

        std::ranges::fill(dp, 0);
        for (u32 u = l; u < r; ++u) dp[u] = u64{1} << (u - l);
        for (auto &&[u, v] : f) dp[u] |= dp[v];
        for (u32 u = 0; u < n; ++u) ans += std::popcount(dp[u]);
    }

    std::cout << u64(n) * (n + 1) / 2 - ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
