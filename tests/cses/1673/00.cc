#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/bellman_ford.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, i64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v;
        i32 w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, std::pair<u32, i64>{--v, -w});
    }

    mld::static_csr g(n, e);

    auto [dst, _] = mld::bellman_ford(g, 0);
    std::cout << (dst[n - 1] == std::numeric_limits<i64>::min() ? -1 : -dst[n - 1]) << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
