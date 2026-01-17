#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/hld.h"
#include "lib/prelude.h"

// Remove all constexpr from hld.h
// to make it compile on cses
void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::dynamic_graph<u32> g(n);
    g.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        g.add_edge(--p, u);
    }

    mld::hld h(g);
    while (q--) {
        u32 u, k;
        std::cin >> u >> k;

        if (auto x = h.jump(--u, k))
            std::cout << *x + 1 << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
