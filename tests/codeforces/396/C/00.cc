#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_sum.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_aggregate.h"
#include "lib/static_csr.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_aggregate<mld::dual_fenwick_tree<mld::actions::add_sum<Z>>> ra(
        g, std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));
    mld::rooted_tree_aggregate<mld::dual_fenwick_tree<mld::actions::add_sum<Z>>> rb(
        g, std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t, u;
        std::cin >> t >> u;

        --u;
        if (t == 1) {
            u32 x, k;
            std::cin >> x >> k;

            ra.apply_subtree(u, x + Z(k) * ra.dep[u]), rb.apply_subtree(u, k);
        } else {
            std::cout << (ra.get(u).val() - rb.get(u).val() * ra.dep[u]).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
