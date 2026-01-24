#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

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

    u64 ans = 0;
    for (u32 i = 0; i < 20; ++i) {
        std::vector<u32> dp(n), kp(n), tp(n);
        for (u32 u = 0; u < n; ++u) dp[u] = (a[u] >> i) & 1;

        auto b = dp;
        for (u32 u : h.tor | std::views::drop(1) | std::views::reverse)
            tp[u] = b[h.par[u]] ? h.siz[u] - dp[u] : dp[u], dp[h.par[u]] += tp[u];

        for (u32 u : h.tor | std::views::drop(1)) {
            kp[u] = kp[h.par[u]] + dp[h.par[u]] - tp[u];
            if (b[u]) kp[u] = n - h.siz[u] - kp[u];
        }

        u64 c = (std::reduce(dp.begin(), dp.end(), u64{0}) +
                 std::reduce(kp.begin(), kp.end(), u64{0}) +
                 std::reduce(b.begin(), b.end()));
        assert(c % 2 == 0);
        ans += (c << i) >> 1;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
