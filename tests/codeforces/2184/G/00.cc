#include <bits/stdc++.h>

#include "lib/algebra/min.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::segment_tree<mld::algebra::min<u32>> st(std::views::iota(u32{0}, n) |
                                                 std::views::transform([](auto) {
                                                     u32 a;
                                                     std::cin >> a;
                                                     return a;
                                                 }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 i, x;
            std::cin >> i >> x;

            st.set(--i, x);
        } else {
            u32 l, r;
            std::cin >> l >> r;

            --l;
            i32 lo = -1, hi = r - l;
            while (hi - lo > 1) {
                u32 d = std::midpoint(lo, hi);
                if (st.fold(l, l + d + 1).val() <= d)
                    hi = d;
                else
                    lo = d;
            }

            std::cout << (hi + l < r && st.fold(l, l + hi + 1) == hi) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
