#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);

    constexpr u32 UNSET = -1;
    std::vector<u32> dep(n, UNSET);
    auto dfs = [&](auto &&self, u32 u) -> void {
        for (u32 v : g[u]) {
            if (dep[v] != UNSET) continue;

            dep[v] = dep[u] + 1;
            self(self, v);
        }
    };

    for (u32 u = 0; u < n; ++u)
        if (dep[u] == UNSET) dfs(dfs, u);

    for (u32 i = 0; i < e.size(); i += 2) {
        auto &&[u, v] = e[i];
        if (dep[u] > dep[v]) std::swap(u, v);

        std::cout << u + 1 << ' ' << v + 1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
