#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    mld::union_find uf(n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        if (uf.merge(--u, --v)) e.emplace_back(u, v), e.emplace_back(v, u);
    }

    if (n == 1) {
        while (k--) std::cout << 1 << ' ' << 1 << '\n';
        return;
    }

    mld::static_csr g(n, e);
    mld::rooted_tree r(g);

    std::vector<u32> tor;
    tor.reserve(6 * n);

    for (u32 u : r.tor) {
        tor.push_back(u);
        for (u32 v = u; v != 0 && r.tor[r.tin[v] + r.siz[v] - 1] == u; v = r.par[v])
            tor.push_back(r.par[v]);
    }

    for (u32 i = 1; i < 2 * n - 1; ++i) tor.push_back(tor[i]);
    for (u32 i = 1; i < 2 * n - 1; ++i) tor.push_back(tor[i]);

    u32 z = (2 * n + k - 1) / k;
    for (u32 i = 0; i < k; ++i) {
        std::cout << z;
        for (u32 u : std::span(tor.begin() + i * z, tor.begin() + (i + 1) * z))
            std::cout << ' ' << u + 1;

        std::cout << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
