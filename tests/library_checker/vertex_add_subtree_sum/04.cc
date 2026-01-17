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

    std::vector<u32> par(n);
    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        dct.link(par[u] = p, u);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, x;
            std::cin >> u >> x;

            dct.query([u, x](auto &&am, u32) { am.add(u, x); });
        } else {
            u32 u;
            std::cin >> u;

            if (u != 0) dct.cut(u, par[u]);
            dct.query([u](auto &&am, u32) { std::cout << am.fold(u).val() << '\n'; });
            if (u != 0) dct.link(u, par[u]);
        }
    }

    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
