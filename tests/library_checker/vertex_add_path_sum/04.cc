#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_sum.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u64> z(n);
    for (auto &&x : z) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        e.emplace_back(a, b);
        e.emplace_back(b, a);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    std::vector<u64> dp(z.begin(), z.end());
    for (u32 u : h.tor | std::views::drop(1)) dp[u] += dp[h.par[u]];

    mld::dual_fenwick_tree<mld::actions::add_sum<u64>> st(
        h.tor | std::views::transform([&](u32 u) { return dp[u]; }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            z[p] += x;
            st.apply(h.tin[p], h.tin[p] + h.siz[p], x);
        } else {
            u32 u, v;
            std::cin >> u >> v;

            u32 w = h.lca(u, v);
            std::cout << (st.get(h.tin[u]).val() + st.get(h.tin[v]).val() + z[w] -
                          2 * st.get(h.tin[w]).val())
                      << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
