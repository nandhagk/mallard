#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/dynamic_graph.h"
#include "lib/tree_diameter.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::dynamic_graph<std::pair<u32, u64>> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        g.add_edge(u, v, w);
        g.add_edge(v, u, w);
    }

    auto [s, par, dst] = mld::tree_diameter(g);

    u32 t = static_cast<u32>(std::ranges::max_element(dst) - dst.begin());

    u32 k = 1;
    for (u32 cur = t; cur != s; cur = par[cur], ++k);

    std::cout << dst[t] << ' ' << k << '\n';
    for (u32 cur = t; cur != s; cur = par[cur]) std::cout << cur << ' ';
    std::cout << s;
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
