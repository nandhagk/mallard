#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/bipartition.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    if (auto col = mld::bipartition(g))
        for (auto &&x : *col) std::cout << x + 1 << ' ';
    else
        std::cout << "IMPOSSIBLE\n";
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
