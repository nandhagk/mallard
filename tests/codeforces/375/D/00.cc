#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/rooted_tree.h"
#include "lib/offline/mo.h"

static constexpr u32 N = 100'000;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> c(n);
    for (auto &&x : c) std::cin >> x;

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

    std::vector<u32> f;
    f.reserve(m);

    mld::offline::mo mo(n);
    mo.reserve(m);
    for (u32 i = 0; i < m; ++i) {
        u32 v, k;
        std::cin >> v >> k;

        --v;
        f.push_back(k);
        mo.query(h.tin[v], h.tin[v] + h.siz[v]);
    }

    std::vector<u32> ans(m);

    std::vector<u32> cnt(N + 1), dp(N + 1);
    mo.solve([&](u32 i) { ++dp[++cnt[c[h.tor[i]]]]; },
             [&](u32 i) { --dp[cnt[c[h.tor[i]]]--]; },
             [&](u32 i) { ans[i] = dp[f[i]]; });

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
