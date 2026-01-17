#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m + 1);

    std::vector<u32> in(n), out(n);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
        ++out[u], ++in[v];
    }

    e.emplace_back(n - 1, 0);
    ++out[n - 1], ++in[0];

    for (u32 u = 0; u < n; ++u) {
        if (in[u] != out[u]) {
            std::cout << "IMPOSSIBLE\n";
            return;
        }
    }

    mld::static_csr g(n, e);

    std::vector<u32> pth;
    pth.reserve(2 * m);

    std::vector<u32> pos(n);
    auto dfs = [&](auto &&self, u32 u) -> void {
        while (out[u]) {
            u32 v = g[u][pos[u]++];
            --out[u], --in[v];
            self(self, v);
            pth.push_back(u);
        }
    };

    dfs(dfs, n - 1);
    if (std::ranges::any_of(in, [](u32 d) { return d; }) &&
        std::ranges::any_of(out, [](u32 d) { return d; })) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    pth.pop_back();
    for (auto &&x : pth | std::views::reverse) std::cout << x + 1 << ' ';
    std::cout << n;
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
