#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/foldable_union_find.h"
#include "lib/offline/incremental_scc.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::foldable_union_find<mld::algebra::sum<Z>> fuf(std::views::iota(u32{0}, n) |
                                                       std::views::transform([](auto) {
                                                           u32 a;
                                                           std::cin >> a;
                                                           return a;
                                                       }));

    mld::offline::incremental_scc isc(n);
    isc.reserve(m);

    Z ans = 0;
    auto f = [&](u32) { std::cout << ans.val() << '\n'; };

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        isc.link(u, v);
        isc.query(f);
    }

    isc.solve([&](u32 u, u32 v) {
        if (!fuf.is_same(u, v)) {
            ans += fuf.fold(u).val() * fuf.fold(v).val();
            fuf.merge(u, v);
        }
    });
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
