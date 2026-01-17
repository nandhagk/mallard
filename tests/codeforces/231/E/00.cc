#include <bits/stdc++.h>

#include "lib/algebra/prod.h"
#include "lib/becc.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

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
    auto [k, ids] = mld::becc(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(2 * k);

    std::vector<u32> a(k);
    for (u32 u = 0; u < n; ++u) ++a[ids[u]];
    for (auto &&x : a) x = std::min<u32>(x, 2);

    for (auto &&[u, v] : e)
        if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);

    mld::static_csr h(k, f);
    mld::hld_aggregate<mld::scan_table<mld::algebra::prod<Z>>> hag(h, a);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        std::cout << hag.fold_path(ids[--u], ids[--v]).val().val() << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
