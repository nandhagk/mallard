#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"
#include "lib/top_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::top_tree<mld::algebra::sum<u64>> tt(std::views::iota(u32{0}, n) |
                                             std::views::transform([](auto) {
                                                 u32 a;
                                                 std::cin >> a;
                                                 return a;
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
            u32 u, v, w, x;
            std::cin >> u >> v >> w >> x;

            tt.cut(u, v);
            tt.link(w, x);
        } else if (t == 1) {
            u32 u, x;
            std::cin >> u >> x;

            tt.add(u, x);
        } else {
            u32 u, p;
            std::cin >> u >> p;

            std::cout << tt.fold_subtree(p, u).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
