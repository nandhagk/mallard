#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_sum.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using action = mld::actions::add_sum<u64>;
    mld::dual_fenwick_tree<action> st(std::views::iota(u32{0}, n) |
                                      std::views::transform([](auto) {
                                          u32 a;
                                          std::cin >> a;
                                          return a;
                                      }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 a, b, u;
            std::cin >> a >> b >> u;

            st.apply(--a, b, u);
        } else {
            u32 k;
            std::cin >> k;

            std::cout << st.get(--k).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
