#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"

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

    std::vector<u32> us;
    us.reserve(n);

    mld::rooted_tree h(g);
    for (u32 u : h.tor | std::views::reverse)
        if (g[u].size() == 1) us.push_back(u);

    if (us.size() % 2) us.push_back(us[0]);

    u32 k = static_cast<u32>(us.size());
    std::cout << k / 2 << '\n';

    for (u32 i = 0; i < k / 2; ++i)
        std::cout << us[i] + 1 << ' ' << us[i + k / 2] + 1 << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
