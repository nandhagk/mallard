#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/affine_sum.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using action = mld::actions::affine_sum<Z>;
    mld::lazy_segment_tree<action> st(std::views::iota(static_cast<u32>(0), n) |
                                      std::views::transform([](auto) {
                                          u32 a;
                                          std::cin >> a;
                                          return a;
                                      }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 l, r, a, b;
            std::cin >> l >> r >> a >> b;

            st.apply(l, r, {a, b});
        } else {
            u32 i;
            std::cin >> i;

            std::cout << st.get(i).val().val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
