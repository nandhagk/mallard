#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/segment_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::min<u32>;
    mld::segment_tree<operand> st(std::views::iota(u32{0}, n) |
                                      std::views::transform([](auto) {
                                          u32 a;
                                          std::cin >> a;
                                          return a;
                                      }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 k, x;
            std::cin >> k >> x;

            st.set(--k, x);
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
