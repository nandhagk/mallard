#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dominator_tree.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, s;
    std::cin >> n >> m >> s;

    mld::dynamic_graph<u32> g(n);
    g.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        g.add_edge(u, v);
    }

    auto par = mld::dominator_tree(g, s);

    par[s] = s;
    for (auto &&x : par) std::cout << i32(x) << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
