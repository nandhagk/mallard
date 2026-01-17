#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> e;
    e.reserve(m);

    mld::am_tree<u32> amt(n);

    u64 ans = 0;
    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v, w);

        if (auto res = amt.insert(u, v, w)) {
            if (auto r = *res) ans -= *r;
            ans += w;
        }
    }

    for (auto &&[u, v, w] : e) {
        u32 z = *amt.max_path(u, v);
        std::cout << (w <= z ? ans : ans + w - z) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
