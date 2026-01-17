#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/enumerate_c3.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> z(n);
    for (auto &&x : z) std::cin >> x;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    u128 ans = 0;
    auto f = [&](u32 u, u32 v, u32 w) { ans += static_cast<u128>(z[u]) * z[v] * z[w]; };

    enumerate_c3(g, f);
    std::cout << static_cast<u32>(ans % 998'244'353) << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
