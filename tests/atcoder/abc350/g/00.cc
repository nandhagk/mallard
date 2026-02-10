#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/combined.h"
#include "lib/algebra/lxor.h"
#include "lib/algebra/sum.h"
#include "lib/link_cut_tree.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand =
        mld::algebra::combined<mld::algebra::sum<u32>, mld::algebra::lxor<u32>>;
    mld::link_cut_tree<operand> lct(
        std::views::iota(u32{0}, n) |
        std::views::transform([](u32 u) { return operand(1, u); }));

    for (u32 x = 0; q--;) {
        u32 a, b, c;
        std::cin >> a >> b >> c;

        u32 t = (Z(a) * (1 + x)).val() % 2;
        u32 u = (Z(b) * (1 + x)).val() % n;
        u32 v = (Z(c) * (1 + x)).val() % n;

        if (t == 0) {
            lct.link(u, v);
        } else {
            if (lct.is_connected(u, v)) {
                auto [z, y] = lct.fold_path(u, v).val();
                if (z == 3) {
                    x = 1 + (u ^ v ^ y);
                } else {
                    x = 0;
                }
            } else {
                x = 0;
            }

            std::cout << x << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
