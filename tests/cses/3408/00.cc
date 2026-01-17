#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    std::vector<std::tuple<u32, u32, u32>> e;
    e.reserve(m);

    mld::am_tree<u64> amt(n);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v, w);
        amt.insert(u, v, static_cast<u64>(w) << 30 | q);
    }

    while (q--) {
        u32 k;
        std::cin >> k;

        bool ans = true;
        while (k--) {
            u32 p;
            std::cin >> p;

            if (ans) {
                auto &&[u, v, w] = e[--p];
                ans = amt.insert(u, v, static_cast<u64>(w) << 30 | q).has_value();
            }
        }

        std::cout << (ans ? "YES\n" : "NO\n");
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
