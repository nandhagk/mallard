#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::affine<Z>;
    mld::segment_tree<operand> st(std::views::iota(u32{0}, n) |
                                  std::views::transform([](auto) {
                                      u32 a, b;
                                      std::cin >> a >> b;
                                      return operand(a, b);
                                  }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 i, c, d;
            std::cin >> i >> c >> d;

            st.set(i, {c, d});
        } else {
            u32 l, r, x;
            std::cin >> l >> r >> x;

            std::cout << st.fold(l, r)(x).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
