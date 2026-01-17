#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/hopcroft_karp.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 l, r, m;
    std::cin >> l >> r >> m;

    u32 n = l + r;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(u, v + l);
        e.emplace_back(v + l, u);
    }

    mld::static_csr g(n, e);

    std::vector<u32> col(n);
    std::fill(col.begin() + l, col.end(), 1);

    auto mat = mld::hopcroft_karp(g, col);

    constexpr u32 UNSET = -1;
    std::cout << (n - std::ranges::count(mat, UNSET)) / 2 << '\n';
    for (u32 u = 0; u < l; ++u)
        if (mat[u] != UNSET) std::cout << u << ' ' << mat[u] - l << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
