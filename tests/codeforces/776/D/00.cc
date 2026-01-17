#include <bits/stdc++.h>

#include "lib/bipartition.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 0; i < m; ++i) {
        u32 k;
        std::cin >> k;

        while (k--) {
            u32 u;
            std::cin >> u;

            e.emplace_back(--u, i);
        }
    }

    mld::static_csr g(n, e);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(4 * n);

    u32 k = m;
    for (u32 i = 0; i < n; ++i) {
        assert(g[i].size() == 2);
        u32 u = g[i][0], v = g[i][1];
        if (a[i]) {
            f.emplace_back(u, k), f.emplace_back(k, u);
            f.emplace_back(v, k), f.emplace_back(k, v);
            ++k;
        } else {
            f.emplace_back(u, v), f.emplace_back(v, u);
        }
    }

    mld::static_csr h(k, f);
    std::cout << (mld::bipartition(h) ? "YES\n" : "NO\n");
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
