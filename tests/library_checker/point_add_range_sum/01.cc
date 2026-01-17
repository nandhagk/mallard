#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/segment_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::segment_tree<mld::algebra::sum<u64>> st(std::views::iota(u32{0}, n) |
                                                 std::views::transform([](auto) {
                                                     u32 a;
                                                     std::cin >> a;
                                                     return a;
                                                 }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            st.add(p, x);
        } else {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << st.fold(l, r).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
