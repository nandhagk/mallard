#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/top_tree.h"
#include "lib/prelude.h"

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
        u32 t, u, v;
        std::cin >> t >> u >> v;

        if (t == 0) {
            u32 a, b;
            std::cin >> a >> b;

            tt.cut(u, v);
            tt.link(a, b);
        } else if (t == 1) {
            tt.add(u, v);
        } else {
            std::cout << tt.fold_path(u, v).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
