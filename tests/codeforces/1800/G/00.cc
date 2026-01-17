#include <bits/stdc++.h>

#include "lib/pbds.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/rooted_tree_isomorphism.h"
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
    mld::rooted_tree h(g, 0);

    mld::pbds::hash_map<u64, u32> m;
    m.resize(n);

    auto iso = mld::rooted_tree_isomorphism(m, g, 0);

    u32 z = 0;
    for (u32 u : h.tor) {
        if (!h.is_ancestor(z, u)) continue;

        std::map<u32, u32> x;
        for (u32 v : g[u])
            if (v != h.par[u]) ++x[iso[v]];

        u32 y = (g[u].size() + (u != 0)) % 2;
        if (std::ranges::count_if(x, [](auto &&p) { return p.second % 2; }) != y) {
            std::cout << "NO\n";
            return;
        }

        if (y == 0) {
            std::cout << "YES\n";
            return;
        }

        for (u32 v : g[u])
            if (v != h.par[u] && x[iso[v]] % 2) z = v;
    }

    std::cout << "YES\n";
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
