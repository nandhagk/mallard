#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/rooted_tree.h"

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

    u32 r = 0;
    for (; r < n; ++r)
        if (g[r].size() == 2) break;

    if (r == n) {
        std::cout << "NO\n";
        return;
    }

    std::cout << "YES\n";

    u32 t = g[r][1];
    mld::rooted_tree h(g, r);

    for (u32 u = 0; u < n; ++u) {
        if (u == r) continue;

        u32 a = u, b = h.par[u];
        if (h.is_ancestor(t, a) ^ (h.dep[u] % 2)) std::swap(a, b);

        std::cout << a + 1 << ' ' << b + 1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
