#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/link_cut_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::link_cut_tree<mld::algebra::sum<u64>> lct(std::views::iota(u32{0}, n) |
                                                   std::views::transform([](auto) {
                                                       u32 a;
                                                       std::cin >> a;
                                                       return a;
                                                   }));
    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;
        lct.link(a, b);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            lct.add(p, x);
        } else {
            u32 u, v;
            std::cin >> u >> v;

            std::cout << lct.fold_path(u, v).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
