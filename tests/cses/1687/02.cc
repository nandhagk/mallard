#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/static_csr.h"
#include "lib/hld.h"
#include "lib/prelude.h"

// Remove all constexpr from hld.h
// to make it compile on cses
void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::hld h(g);
    while (q--) {
        u32 u, k;
        std::cin >> u >> k;

        if (auto x = h.jump(--u, k))
            std::cout << *x + 1 << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
