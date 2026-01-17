#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u32>>> e;
    e.reserve(2 * m);

    std::vector<u32> deg(n);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, std::pair<u32, u32>{v, i});
        e.emplace_back(v, std::pair<u32, u32>{u, i});
        ++deg[u], ++deg[v];
    }

    if (std::ranges::any_of(deg, [](u32 d) { return d % 2; })) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    mld::static_csr g(n, e);

    std::vector<u32> pth;
    pth.reserve(2 * m);

    std::vector<bool> vis(m);
    std::vector<u32> pos(n);
    auto dfs = [&](auto &&self, u32 u) -> void {
        while (deg[u]) {
            auto [v, i] = g[u][pos[u]++];

            if (vis[i]) continue;
            vis[i] = true;

            --deg[u], --deg[v];
            self(self, v);
            pth.push_back(u);
        }
    };

    dfs(dfs, 0);
    if (std::ranges::count(vis, false)) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    std::cout << 1 << ' ';
    for (auto &&x : pth) std::cout << x + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
