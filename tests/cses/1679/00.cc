#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"
#include "lib/topological_sort.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::dynamic_graph<u32> g(n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        g.add_edge(--u, --v);
    }

    if (auto ord = mld::topological_sort(g))
        for (u32 u : *ord) std::cout << u + 1 << ' ';
    else
        std::cout << "IMPOSSIBLE\n";
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
