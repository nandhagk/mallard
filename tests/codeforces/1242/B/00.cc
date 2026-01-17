#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"
#include "lib/ccc.h"

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
    auto [k, ids] = mld::ccc(g);

    mld::union_find uf(n);

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(k, UNSET);

    for (u32 u = 0; u < n; ++u) {
        if (prv[ids[u]] != UNSET) uf.merge(u, prv[ids[u]]);
        prv[ids[u]] = u;
    }

    u32 ans = 0;
    for (auto &&[u, v] : e) ans += uf.merge(u, v);

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
