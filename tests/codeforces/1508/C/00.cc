#include <bits/stdc++.h>

#include "lib/ccc.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    std::vector<std::tuple<u32, u32, u32>> f;
    e.reserve(2 * m), f.reserve(m);

    u32 z = 0;
    u64 s = u64(n) * (n - 1) / 2 - m;
    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        z ^= w;
        f.emplace_back(w, u, v);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = mld::ccc(g);

    if (s + k != n) z = 0;

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(k, UNSET);

    mld::union_find uf(n);
    for (u32 u = 0; u < n; ++u) {
        if (prv[ids[u]] != UNSET) uf.merge(u, prv[ids[u]]);
        prv[ids[u]] = u;
    }

    std::ranges::sort(f);
    mld::union_find ds(n);

    u64 ans = 0;
    for (auto &&[w, u, v] : f)
        if (uf.merge(u, v))
            ans += w, ds.merge(u, v);
        else if (ds.merge(u, v))
            z = std::min(z, w);

    std::cout << ans + z << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
