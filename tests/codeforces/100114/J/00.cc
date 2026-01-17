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
    auto [z, ids] = mld::becc(g);

    if (z == 1) {
        std::cout << 1 << ' ' << 2 << '\n';
        return;
    }

    std::vector<u32> rid(z);
    for (u32 u = 0; u < n; ++u) rid[ids[u]] = u;

    for (auto &&[u, v] : e)
        if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);

    mld::static_csr h(z, f);
    auto [s, _, dst] = mld::tree_diameter(h);

    u32 t = static_cast<u32>(std::ranges::max_element(dst) - dst.begin());
    std::cout << rid[s] + 1 << ' ' << rid[t] + 1 << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifndef NANDHAGK_LOCAL
    std::ifstream in("input.txt");
    std::cin.rdbuf(in.rdbuf());

    std::ofstream out("output.txt");
    std::cout.rdbuf(out.rdbuf());
#endif

    solve();
}
