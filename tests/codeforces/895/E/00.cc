#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/actions/affine_sum.h"
#include "lib/lazy_segment_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::lazy_segment_tree<mld::actions::affine_sum<f80>> st(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
        u32 a;
        std::cin >> a;
        return a;
    }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 l1, r1, l2, r2;
            std::cin >> l1 >> r1 >> l2 >> r2;

            --l1, --l2;
            f80 x = st.fold(l1, r1).val() / (r1 - l1);
            f80 y = st.fold(l2, r2).val() / (r2 - l2);

            st.apply(l1, r1, {f80(r1 - l1 - 1) / (r1 - l1), y / (r1 - l1)});
            st.apply(l2, r2, {f80(r2 - l2 - 1) / (r2 - l2), x / (r2 - l2)});
        } else {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << st.fold(--l, r).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << std::fixed;
    std::cout << std::setprecision(12);

    solve();
}
