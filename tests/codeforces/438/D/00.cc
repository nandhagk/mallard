#include <bits/stdc++.h>

#include "lib/algebra/combined.h"
#include "lib/algebra/max.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    using operand =
        mld::algebra::combined<mld::algebra::sum<u64>, mld::algebra::max<u32>>;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::segment_tree<operand> st(
        a | std::views::transform([](u32 x) { return operand(x, x); }));

    while (m--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << std::get<0>(st.fold(--l, r).val()) << '\n';
        } else if (t == 2) {
            u32 l, r, x;
            std::cin >> l >> r >> x;

            --l;
            auto f = [&](auto &&y) { return std::get<1>(y.val()) < x; };
            for (u32 k; (k = st.max_right(l, f)) < r;)
                a[k] %= x, st.set(k, operand(a[k], a[k]));
        } else {
            u32 k, x;
            std::cin >> k >> x;

            a[--k] = x;
            st.set(k, operand(a[k], a[k]));
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
