#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/spfa.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, i64>>> e;
    e.reserve(m + n);

    while (m--) {
        u32 u, v;
        i32 w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, std::pair<u32, i64>{--v, w});
    }

    for (u32 u = 0; u < n; ++u) e.emplace_back(n, std::pair<u32, i64>{u, 0});
    mld::static_csr g(n + 1, e);

    auto [dst, prv] = mld::spfa(g, n);

    std::vector<u32> cyc;
    cyc.reserve(n);

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u) {
        if (dst[u] != std::numeric_limits<i64>::min()) continue;

        u32 t = u;
        for (u32 i = 0; i < n && prv[t] != UNSET; ++i) t = prv[t];

        if (prv[t] == UNSET) continue;

        cyc.clear();
        for (u32 s = t;; s = prv[s]) {
            cyc.push_back(s);
            if (s == t && cyc.size() > 1) break;
        }

        std::cout << "YES" << '\n';
        for (u32 s : cyc | std::views::reverse) std::cout << s + 1 << ' ';
        std::cout << '\n';

        return;
    }

    std::cout << "NO\n";
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
