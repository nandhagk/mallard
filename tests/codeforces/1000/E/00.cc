#include <bits/stdc++.h>

#include "lib/becc.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/tree_diameter.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e, f;
    e.reserve(2 * m), f.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = mld::becc(g);

    for (auto &&[u, v] : e)
        if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);

    mld::static_csr h(k, f);
    auto [_, __, dst] = mld::tree_diameter(h);

    std::cout << *std::ranges::max_element(dst) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
