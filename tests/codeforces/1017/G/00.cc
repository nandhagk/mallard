#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/assign.h"
#include "lib/algebra/mxprefsuff.h"
#include "lib/hld_aggregate.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

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

    mld::static_csr g(n, e);

    using operand = mld::algebra::mxprefsuff<i64>;
    mld::hld_aggregate<mld::lazy_segment_tree<mld::actions::assign<operand>>> st(
        g, std::views::iota(u32{0}, n) |
               std::views::transform([](auto) { return operand(-1); }));

    while (q--) {
        u32 t, u;
        std::cin >> t >> u;

        --u;
        if (t == 1) {
            st.set(u, operand(std::get<0>(st.get(u).val()) + 1));
        } else if (t == 2) {
            i64 y = std::get<0>(st.get(u).val());
            st.apply_subtree(u, operand(-1));
            st.set(u, operand(y));

            i64 x = std::get<1>(st.fold_path(u, 0).val());
            if (x >= 0) st.set(u, operand(y - x - 1));
        } else {
            i64 x = std::get<1>(st.fold_path(u, 0).val());
            std::cout << (x < 0 ? "white" : "black") << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
