#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
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
    mld::rooted_tree_lca h(g);

    u32 q;
    std::cin >> q;

    std::vector<u32> x;
    x.reserve(n);

    constexpr u32 UNSET = -1;
    while (q--) {
        u32 k;
        std::cin >> k;

        x.clear();
        while (k--) {
            u32 u;
            std::cin >> u;
            x.emplace_back(--u);
        }

        std::ranges::sort(x, [&](u32 u, u32 v) { return h.dep[u] > h.dep[v]; });

        u32 s = x[0], t = UNSET;
        for (u32 u : x) {
            if (!h.is_ancestor(u, s)) {
                t = u;
                goto found;
            }
        }

        std::cout << "YES\n";
        continue;

    found:;
        std::cout << (std::ranges::all_of(x,
                                          [&](u32 u) { return h.is_on_path(s, t, u); })
                          ? "YES\n"
                          : "NO\n");
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
