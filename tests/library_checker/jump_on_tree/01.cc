#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/hld.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        g.add_edge(a, b);
        g.add_edge(b, a);
    }

    mld::hld h(g);
    while (q--) {
        u32 u, v, k;
        std::cin >> u >> v >> k;

        if (auto x = h.jump(u, v, k))
            std::cout << *x << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
