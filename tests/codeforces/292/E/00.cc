#include <bits/stdc++.h>

#include "lib/actions/assign.h"
#include "lib/algebra/affine.h"
#include "lib/dual_segment_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<i32> a(n), b(n);
    for (auto &&x : a) std::cin >> x;
    for (auto &&x : b) std::cin >> x;

    using operand = mld::algebra::affine<i32>;
    mld::dual_segment_tree<mld::actions::assign<operand>> st(
        std::views::iota(u32{0}, n) |
        std::views::transform([](auto) { return operand(0, 0); }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 x, y, k;
            std::cin >> x >> y >> k;

            --x, --y;
            st.apply(y, y + k, operand(1, x - y));
        } else {
            u32 x;
            std::cin >> x;

            auto [u, v] = st.get(--x).val();
            std::cout << (u ? a[x + v] : b[x]) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
