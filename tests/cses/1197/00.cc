#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, i32>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v;
        i32 w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, std::pair<u32, i32>{--v, w});
    }

    mld::static_csr g(n, e);

    std::vector<i64> dst(n);
    std::vector<u32> prv(n);

    for (u32 i = 0; i < n - 1; ++i) {
        for (u32 u = 0; u < n; ++u) {
            for (auto &&[v, w] : g[u])
                if (dst[u] + w < dst[v]) dst[v] = dst[u] + w, prv[v] = u;
        }
    }

    std::vector<u32> cycle;
    cycle.reserve(n);
    for (u32 u = 0; u < n; ++u) {
        for (auto &&[v, w] : g[u]) {
            if (dst[u] + w >= dst[v]) continue;

            u32 t = v;
            for (u32 i = 0; i < n; ++i) t = prv[t];

            cycle.clear();
            for (u32 s = t;; s = prv[s]) {
                cycle.push_back(s);
                if (s == t && cycle.size() > 1) break;
            }

            std::cout << "YES" << '\n';
            for (u32 s : cycle | std::views::reverse) std::cout << s + 1 << ' ';
            std::cout << '\n';

            return;
        }
    }

    std::cout << "NO" << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
