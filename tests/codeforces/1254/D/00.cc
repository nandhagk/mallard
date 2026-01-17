#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_sum.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

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
    mld::hld_aggregate<mld::dual_fenwick_tree<mld::actions::add_sum<Z>>> hag(
        g, std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    std::vector<Z> mrk(n);

    Z den = Z(n).inv();
    constexpr u32 UNSET = -1;
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 v, d;
            std::cin >> v >> d;

            mrk[--v] += d;
            hag.apply_subtree(v, d);
            hag.apply_outtree(v, (den * hag.siz[v]) * d);

            if (u32 u = hag.bst[v]; u != UNSET)
                hag.apply_subtree(u, -((den * hag.siz[u]) * d));
        } else {
            u32 v;
            std::cin >> v;

            Z ans = hag.get(--v).val();
            for (u32 u = hag.beg[v]; u != 0; u = hag.beg[hag.par[u]])
                ans -= (den * hag.siz[u]) * mrk[hag.par[u]];

            std::cout << ans.val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
