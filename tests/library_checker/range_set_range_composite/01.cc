#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/assign.h"
#include "lib/algebra/affine.h"
#include "lib/dynamic_lazy_segment_tree.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::affine<Z>;
    using action = mld::actions::assign<operand>;
    mld::dynamic_lazy_segment_tree<action, mld::managers::clonable<>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a, b;
            std::cin >> a >> b;
            return operand(a, b);
        }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 l, r, a, b;
            std::cin >> l >> r >> a >> b;

            st.apply(l, r, operand(a, b).val());
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
