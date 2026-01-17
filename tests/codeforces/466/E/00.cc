#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> par(n);
    std::iota(par.begin(), par.end(), 0);

    auto find = [&](u32 u) {
        for (; par[u] != u; u = par[u] = par[par[u]]);
        return u;
    };

    std::vector<std::pair<u32, u32>> p, q, e;
    p.reserve(m), q.reserve(m), e.reserve(n);

    while (m--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 x, y;
            std::cin >> x >> y;

            par[--x] = --y;
            e.emplace_back(y, x);
        } else if (t == 2) {
            u32 x;
            std::cin >> x;

            --x;
            p.emplace_back(x, find(x));
        } else {
            u32 x, i;
            std::cin >> x >> i;

            q.emplace_back(--x, --i);
        }
    }

    for (u32 u = 0; u < n; ++u)
        if (par[u] == u) e.emplace_back(n, u);

    mld::static_csr g(n + 1, e);
    mld::rooted_tree h(g, n);

    for (auto &&[x, i] : q) {
        auto &&[u, v] = p[i];
        std::cout << ((h.is_ancestor(x, u) && h.is_ancestor(v, x)) ? "YES\n" : "NO\n");
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
