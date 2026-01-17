#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dominator_tree.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, s;
    std::cin >> n >> m >> s;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(u, v);
    }

    mld::static_csr g(n, e);
    auto par = mld::dominator_tree(g, s, m);

    par[s] = s;
    for (auto &&x : par) std::cout << i32(x) << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
