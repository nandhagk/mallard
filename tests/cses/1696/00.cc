#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/hopcroft_karp.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * k);

    while (k--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v + n);
        e.emplace_back(v + n, u);
    }

    mld::static_csr g(n + m, e);

    std::vector<u32> col(n + m);
    std::fill(col.begin() + n, col.end(), 1);

    auto mat = mld::hopcroft_karp(g, col);

    constexpr u32 UNSET = -1;
    std::cout << (n + m - std::ranges::count(mat, UNSET)) / 2 << '\n';
    for (u32 u = 0; u < n; ++u)
        if (mat[u] != UNSET) std::cout << u + 1 << ' ' << mat[u] - n + 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
