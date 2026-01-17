#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/dynamic_graph.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_distinct.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> z(n);
    for (auto &&x : z) std::cin >> x;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        --a, --b;
        g.add_edge(a, b);
        g.add_edge(b, a);
    }

    mld::hld h(g);

    auto &&[v, na] =
        mld::compress(h.tor | std::views::transform([&](u32 k) { return z[k]; }));

    mld::static_distinct sd(na, static_cast<u32>(v.size()));
    for (u32 u = 0; u < n; ++u)
        std::cout << sd.count(h.tin[u], h.tin[u] + h.siz[u]) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
