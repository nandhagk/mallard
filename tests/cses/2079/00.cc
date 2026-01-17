#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/dynamic_graph.h"
#include "lib/ctd.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    auto [root, _] = mld::ctd(g);
    std::cout << root + 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
