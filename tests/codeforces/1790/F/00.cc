#include <bits/stdc++.h>

#include "lib/ctd.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, s;
    std::cin >> n >> s;

    --s;

    std::vector<u32> c(n - 1);
    for (auto &&x : c) std::cin >> x, --x;

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

    constexpr u32 UNSET = -1;
    auto [_, par] = mld::ctd(g);

    std::vector<u32> bst(n, n);

    bst[s] = 0;
    for (u32 v = par[s]; v != UNSET; v = par[v]) bst[v] = h.dist(s, v);

    u32 ans = n;
    for (u32 u : c) {
        ans = std::min(ans, bst[u]);
        bst[u] = 0;
        for (u32 v = par[u]; v != UNSET; v = par[v]) {
            ans = std::min(ans, bst[v] + h.dist(u, v));
            bst[v] = std::min(bst[v], h.dist(u, v));
        }

        std::cout << ans << ' ';
    }

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
