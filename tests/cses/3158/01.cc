#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dijkstra.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> c(m);
    for (auto &&x : c) std::cin >> x;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(400'005);

    for (u32 i = 0; i < m; ++i) {
        u32 k;
        std::cin >> k;

        while (k--) {
            u32 u;
            std::cin >> u;

            --u;
            e.emplace_back(u, std::pair<u32, u64>{i + n, c[i]});
            e.emplace_back(i + n, std::pair<u32, u64>{u, c[i]});
        }
    }

    mld::static_csr g(n + m, e);
    auto [dst, _] = dijkstra(g, 0);

    for (auto &&x : dst | std::views::take(n)) std::cout << x / 2 << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
