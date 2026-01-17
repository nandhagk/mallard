#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine_rev.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"
#include "lib/top_tree.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::affine_rev<Z>;
    mld::top_tree<operand> tt(std::views::iota(u32{0}, n) |
                              std::views::transform([](auto) {
                                  u32 b, c;
                                  std::cin >> b >> c;
                                  return operand(b, c);
                              }));

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;
        tt.link(u, v);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, v, a, b;
            std::cin >> u >> v >> a >> b;

            tt.cut(u, v);
            tt.link(a, b);
        } else if (t == 1) {
            u32 p, c, d;
            std::cin >> p >> c >> d;
            tt.set(p, operand(c, d));
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
