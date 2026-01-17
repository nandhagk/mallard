#include <bits/stdc++.h>

#include "lib/dynamic_graph.h"
#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u64> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::dynamic_graph<u32> g(n), h(n);
    g.reserve(m), h.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g.add_edge(u, v);
        h.add_edge(v, u);
    }

    auto [k, ids] = mld::scc(g);

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, [&](u32 u, u32 v) {
        return std::tie(ids[u], a[u], u) < std::tie(ids[v], a[v], v);
    });

    Z ans = 0;
    std::vector<std::map<u64, Z>> dp(n);
    for (u32 u : ord) {
        for (u32 v : h[u]) {
            Z z = a[u] > a[v] ? 1 + dp[v][a[u] - a[v]] : 1;
            ans += z;
            dp[u][a[v]] += z;
        }
    }

    std::cout << ans.val() << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
