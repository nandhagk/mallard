#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/foldable_am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::foldable_am_tree<i32, mld::algebra::sum<u64>> amt(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a;
            std::cin >> a;
            return a;
        }));

    mld::offline::dynamic_connectivity<decltype(amt)> dct(n);
    dct.reserve(n + 2 * q);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        dct.link(u, v);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, v, w, x;
            std::cin >> u >> v >> w >> x;

            dct.cut(u, v);
            dct.link(w, x);
        } else if (t == 1) {
            u32 u, x;
            std::cin >> u >> x;

            dct.query([u, x](auto &&am, u32) { am.add(u, x); });
        } else {
            u32 u, p;
            std::cin >> u >> p;

            dct.cut(u, p);
            dct.query([u](auto &&am, u32) { std::cout << am.fold(u).val() << '\n'; });
            dct.link(u, p);
        }
    }

    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
