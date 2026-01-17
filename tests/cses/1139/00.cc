#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

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

    std::vector<u32> dp(n);
    auto dfs = [&](auto &&self, u32 u, u32 t) -> std::set<u32> {
        std::set<u32> s{z[u]};
        for (u32 v : g[u]) {
            if (v == t) continue;

            auto &&r = self(self, v, u);
            if (r.size() > s.size()) std::swap(r, s);

            s.merge(std::move(r));
        }

        dp[u] = static_cast<u32>(s.size());
        return s;
    };

    dfs(dfs, 0, 0);
    for (u32 u = 0; u < n; ++u) std::cout << dp[u] << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
