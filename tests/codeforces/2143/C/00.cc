#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/topological_sort.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, x, y;
        std::cin >> u >> v >> x >> y;

        --u, --v;
        if (x < y) {
            e.emplace_back(u, v);
        } else {
            e.emplace_back(v, u);
        }
    }

    mld::static_csr g(n, e);

    auto ord = *mld::topological_sort(g);

    std::vector<u32> rnk(n);
    for (u32 u = 0; u < n; ++u) rnk[ord[u]] = u;

    for (u32 u : rnk) std::cout << u + 1 << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
