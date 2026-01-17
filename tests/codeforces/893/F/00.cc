#include <bits/stdc++.h>

#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/sparse_table.h"
#include "lib/static_csr.h"
#include "lib/wavelet_matrix_aggregate.h"

void solve() {
    u32 n, r;
    std::cin >> n >> r;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

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
    mld::rooted_tree h(g, --r);

    mld::wavelet_matrix_aggregate<
        mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>>>
        wma(h.tor | std::views::transform(
                        [&](u32 u) { return std::make_pair(h.dep[u], a[u]); }), n + 1);

    u32 m;
    std::cin >> m;

    for (u32 lst = 0; m--;) {
        u32 p, q;
        std::cin >> p >> q;

        u32 u = (p + lst) % n;
        u32 k = (q + lst) % n;

        auto [_, y] = wma.rank(h.tin[u], h.tin[u] + h.siz[u], std::min(n, k + h.dep[u] + 1));
        lst = y.val();
        std::cout << lst << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
