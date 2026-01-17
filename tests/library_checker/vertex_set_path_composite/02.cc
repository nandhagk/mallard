#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine_rev.h"
#include "lib/top_tree.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::affine_rev<Z>;
    mld::top_tree<operand> tt(std::views::iota(u32{0}, n) |
                                    std::views::transform([](auto) {
                                        u32 a, b;
                                        std::cin >> a >> b;
                                        return operand(a, b);
                                    }));

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;
        tt.link(a, b);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, b, c;
            std::cin >> p >> b >> c;

            tt.set(p, operand(b, c));
        } else {
            u32 u, v, x;
            std::cin >> u >> v >> x;

            std::cout << tt.fold_path(u, v)(x).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
