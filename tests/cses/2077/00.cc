#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/bcc.h"
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

    auto [cut, _] = mld::bcc(g);

    std::cout << std::reduce(cut.begin(), cut.end(), u32(0)) << '\n';
    for (u32 u = 0; u < n; ++u)
        if (cut[u]) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
