#include <bits/stdc++.h>

#include "lib/actions/add_sum.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

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
    mld::rooted_tree h(g);

    mld::dual_fenwick_tree<mld::actions::add_sum<i64>> dft(
        h.tor |
        std::views::transform([&](u32 k) { return h.dep[k] % 2 ? -i64(a[k]) : i64(a[k]); }));

    while (m--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 x, v;
            std::cin >> x >> v;

            --x;
            dft.apply(h.tin[x], h.tin[x] + h.siz[x], h.dep[x] % 2 ? -i64(v) : i64(v));
        } else {
            u32 x;
            std::cin >> x;

            i64 z = dft.get(h.tin[--x]).val();
            std::cout << (h.dep[x] % 2 ? -z : z) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
