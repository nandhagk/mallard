#include <bits/stdc++.h>

#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u <= n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(p, u);
    }

    mld::static_csr g(n + 1, e);
    mld::hld h(g);

    std::vector<std::pair<u32, u32>> d;
    d.reserve(n + 1);
    for (u32 u = 0; u <= n; ++u) d.emplace_back(h.dep[u], h.tin[u]);
    std::ranges::sort(d);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, k;
        std::cin >> u >> k;

        if (h.dep[u] <= k) {
            std::cout << 0 << ' ';
            continue;
        }

        u32 v = *h.jump(u, k);
        u32 l = static_cast<u32>(
            std::ranges::lower_bound(d, std::make_pair(h.dep[u], h.tin[v])) -
            d.begin());
        u32 r = static_cast<u32>(
            std::ranges::lower_bound(d, std::make_pair(h.dep[u], h.tin[v] + h.siz[v])) -
            d.begin());

        std::cout << r - l - 1 << ' ';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
