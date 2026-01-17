#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/hopcroft_karp.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> e(m);
    for (auto &&[u, v, w] : e) std::cin >> u >> v >> w, --u, --v;

    std::vector<std::pair<u32, u32>> f;
    f.reserve(2 * m);

    std::vector<u32> col(2 * n);
    std::fill(col.begin() + n, col.end(), 1);

    constexpr u32 UNSET = -1;

    u32 inf = 1e9 + 5;
    u32 lo = 0, hi = inf;
    while (hi - lo > 1) {
        u32 z = std::midpoint(lo, hi);

        f.clear();
        for (auto &&[u, v, w] : e)
            if (w <= z) f.emplace_back(u, v + n), f.emplace_back(v + n, u);

        mld::static_csr g(2 * n, f);
        auto mat = mld::hopcroft_karp(g, col);

        if (std::ranges::none_of(mat, [](u32 u) { return u == UNSET; }))
            hi = z;
        else
            lo = z;
    }

    std::cout << i32(hi == inf ? -1 : hi) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
