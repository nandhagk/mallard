#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::string s;
    std::cin >> s;

    std::vector<u32> dp(n);
    for (u32 u : h.tor | std::views::reverse) {
        u32 a = 0, b = 0;
        for (u32 v : g[u])
            if (v != h.par[u]) dp[u] += dp[v], a += s[v] == 'P', b += s[v] == 'S';

        if (s[u] == 'P') {
            dp[u] += b;
        } else if (s[u] == 'S') {
            dp[u] += a;
        } else {
            //
            if (a > b) {
                s[u] = 'P';
                dp[u] += b;
            } else if (b > a) {
                s[u] = 'S';
                dp[u] += a;
            } else {
                dp[u] += a;
            }
        }
    }

    std::cout << dp[0] << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
