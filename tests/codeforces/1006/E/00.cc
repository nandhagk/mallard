#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

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
    mld::rooted_tree h(g);

    while (q--) {
        u32 u, k;
        std::cin >> u >> k;

        --u, --k;
        if (k >= h.siz[u])
            std::cout << -1 << '\n';
        else
            std::cout << h.tor[h.tin[u] + k] + 1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
