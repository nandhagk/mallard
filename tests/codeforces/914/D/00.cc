#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/gcd.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::segment_tree<mld::algebra::gcd<u32>> st(std::views::iota(u32{0}, n) |
                                                 std::views::transform([](auto) {
                                                     u32 a;
                                                     std::cin >> a;
                                                     return a;
                                                 }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 l, r, x;
            std::cin >> l >> r >> x;

            if (st.fold(--l, r).val() == x) {
                std::cout << "YES\n";
                continue;
            }

            auto f = [&](auto &&y) { return y.val() % x == 0; };
            u32 a = st.max_right(l, f);
            u32 b = st.min_left(r, f);

            assert(a < b);
            std::cout << (b - a == 1 ? "YES\n" : "NO\n");
        } else {
            u32 p, x;
            std::cin >> p >> x;

            st.set(--p, x);
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
