#include <bits/stdc++.h>

#include "lib/ctd.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

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

    auto [s, par] = mld::ctd(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n);

    for (u32 u = 0; u < n; ++u)
        if (u != s) f.emplace_back(par[u], u);

    mld::static_csr h(n, f);
    mld::rooted_tree r(h, s);

    for (u32 u = 0; u < n; ++u) std::cout << static_cast<char>(r.dep[u] + 'A') << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
