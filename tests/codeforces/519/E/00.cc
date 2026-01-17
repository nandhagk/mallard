#include <bits/stdc++.h>

#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::hld h(g);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        if (h.dep[u] < h.dep[v]) std::swap(u, v);

        if (u == v) {
            std::cout << n << '\n';
            continue;
        }

        u32 w = h.lca(u, v);
        u32 x = h.dep[u] - h.dep[w], y = h.dep[v] - h.dep[w], z = x + y;
        if (z % 2) {
            std::cout << 0 << '\n';
            continue;
        }

        if (x == y) {
            u32 a = *h.jump(u, x - 1), b = *h.jump(v, x - 1);
            std::cout << n - h.siz[a] - h.siz[b] << '\n';
        } else {
            u32 a = *h.jump(u, z / 2), b = *h.jump(u, z / 2 - 1);
            std::cout << h.siz[a] - h.siz[b] << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
