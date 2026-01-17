#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/affine_sum.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using action = mld::actions::affine_sum<u64>;
    mld::lazy_segment_tree<action> st(std::views::iota(u32{0}, n) |
                                      std::views::transform([](auto) {
                                          u32 a;
                                          std::cin >> a;
                                          return a;
                                      }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 a, b, x;
            std::cin >> a >> b >> x;

            st.apply(--a, b, {1, x});
        } else if (t == 2) {
            u32 a, b, x;
            std::cin >> a >> b >> x;

            st.apply(--a, b, {0, x});
        } else {
            u32 a, b;
            std::cin >> a >> b;

            std::cout << st.fold(--a, b).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
