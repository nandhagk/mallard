#include <bits/stdc++.h>

#include "lib/becc.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e, f;
    e.reserve(2 * m), f.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = mld::becc(g);

    std::vector<u32> cnt(k);
    for (u32 u = 0; u < n; ++u) ++cnt[ids[u]];

    std::vector<u32> rid(k);
    for (u32 u = 0; u < n; ++u) rid[ids[u]] = u;

    for (auto &&[u, v] : e)
        if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);

    mld::static_csr h(k, f);

    u32 r = rid[std::ranges::max_element(cnt) - cnt.begin()];

    constexpr u32 UNSET = -1;
    std::vector<u32> dep(n, UNSET), par(n, UNSET);
    auto dfs = [&](auto &&self, u32 u) -> void {
        for (u32 v : g[u]) {
            if (dep[v] != UNSET) continue;

            dep[v] = dep[u] + 1;
            par[v] = u;
            self(self, v);
        }
    };

    dfs(dfs, r);

    std::cout << cnt[ids[r]] << '\n';
    for (u32 i = 0; i < e.size(); i += 2) {
        auto &&[u, v] = e[i];
        if (dep[u] > dep[v]) std::swap(u, v);

        if (par[v] != u)
            std::cout << u + 1 << ' ' << v + 1 << '\n';
        else
            std::cout << v + 1 << ' ' << u + 1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
