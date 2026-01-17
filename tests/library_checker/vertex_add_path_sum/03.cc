#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/static_csr.h"
#include "lib/fenwick_tree.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> z(n);
    for (auto &&x : z) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        e.emplace_back(a, b);
        e.emplace_back(b, a);
    }

    mld::static_csr g(n, e);
    mld::hld_aggregate<mld::fenwick_tree<mld::algebra::sum<u64>>> hag(g, z);
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            hag.add(p, x);
        } else {
            u32 u, v;
            std::cin >> u >> v;

            std::cout << hag.fold_path(u, v).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
