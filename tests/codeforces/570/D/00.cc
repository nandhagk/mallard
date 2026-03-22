#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/lxor.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"
#include "lib/wavelet_matrix_aggregate.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;
        e.emplace_back(--p, u);
    }

    std::string s;
    s.reserve(n + 1);
    std::cin >> s;

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    mld::wavelet_matrix_aggregate<mld::scan_table<mld::algebra::lxor<u32>>> wma(
        std::views::iota(u32{0}, n) | std::views::transform([&](u32 i) {
            u32 u = h.tor[i];
            return std::make_pair(h.dep[u], 1 << (s[u] - 'a'));
        }),
        n);

    while (q--) {
        u32 u, d;
        std::cin >> u >> d;

        --u, --d;
        auto [a, x] = wma.rank(h.tin[u], h.tin[u] + h.siz[u], d);
        auto [b, y] = wma.rank(h.tin[u], h.tin[u] + h.siz[u], d + 1);
        u32 z = (y + (-x)).val();

        std::cout << (std::popcount(z) <= 1 ? "Yes\n" : "No\n");
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
