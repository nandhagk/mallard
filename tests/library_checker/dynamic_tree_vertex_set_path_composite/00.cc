#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine_rev.h"
#include "lib/link_cut_tree.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::affine_rev<Z>;
    mld::link_cut_tree<operand> lct(std::views::iota(u32{0}, n) |
                                    std::views::transform([](auto) {
                                        u32 b, c;
                                        std::cin >> b >> c;
                                        return operand(b, c);
                                    }));

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;
        lct.link(u, v);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, v, a, b;
            std::cin >> u >> v >> a >> b;

            lct.cut(u, v);
            lct.link(a, b);
        } else if (t == 1) {
            u32 p, c, d;
            std::cin >> p >> c >> d;
            lct.set(p, operand(c, d));
        } else {
            u32 u, v, x;
            std::cin >> u >> v >> x;

            std::cout << lct.fold_path(u, v)(x).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
