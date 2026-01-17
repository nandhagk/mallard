#include <algorithm>
#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dominator_tree.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::dynamic_graph<u32> g(n);
    g.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        g.add_edge(--u, --v);
    }

    auto par = mld::dominator_tree(g);
    std::vector<bool> out(n);

    out[0] = true;
    for (u32 u = n - 1; u != 0; u = par[u]) out[u] = true;

    std::cout << std::ranges::count(out, true) << '\n';
    for (u32 u = 0; u < n; ++u)
        if (out[u]) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
