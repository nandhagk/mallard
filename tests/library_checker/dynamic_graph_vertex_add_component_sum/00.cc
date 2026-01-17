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

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, v;
            std::cin >> u >> v;

            dct.link(u, v);
        } else if (t == 1) {
            u32 u, v;
            std::cin >> u >> v;

            dct.cut(u, v);
        } else if (t == 2) {
            u32 u, x;
            std::cin >> u >> x;

            dct.query([u, x](auto &&am, u32) { am.add(u, x); });
        } else {
            u32 u;
            std::cin >> u;

            dct.query([u](auto &&am, u32) { std::cout << am.fold(u).val() << '\n'; });
        }
    }

    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
