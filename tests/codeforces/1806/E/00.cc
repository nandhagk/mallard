#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/mo_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    std::vector<u64> dp(n);
    for (u32 u = 0; u < n; ++u) dp[u] = u64(a[u]) * a[u];

    for (u32 u : h.tor | std::views::drop(1)) dp[u] += dp[h.par[u]];

    mld::offline::mo_tree mo(g);
    mo.reserve(q);
    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        mo.query(--u, --v);
    }

    u64 cur = 0;

    std::vector<u32> cnt(n);
    std::vector<u64> prd(n, 1);

    std::vector<u64> ans(q);
    mo.solve<true>(
        [&](u32 u) {
            u32 d = h.dep[u];

            ++cnt[d], prd[d] *= a[u];
            if (cnt[d] == 2) cur += prd[d];

            assert(cnt[d] <= 2);
        },
        [&](u32 u) {
            u32 d = h.dep[u];
            if (cnt[d] == 2) cur -= prd[d];

            --cnt[d], prd[d] /= a[u];
        },
        [&](u32 k, u32 u, u32 v) { ans[k] = cur + dp[h.lca(u, v)]; });
    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
