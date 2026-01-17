#include <bits/stdc++.h>

#include "lib/algebra/max.h"
#include "lib/am_tree.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    mld::am_tree<u32> amt(n);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        amt.insert(--u, --v, i + 1);
    }

    mld::four_russian<mld::sparse_table<mld::algebra::max<u32>>> agg(
        std::views::iota(u32{0}, n - 1) |
        std::views::transform([&](u32 k) { return *amt.max_path(k, k + 1); }));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        if (l == r) {
            std::cout << 0 << ' ';
        } else {
            std::cout << agg.fold(--l, --r).val() << ' ';
        }
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
