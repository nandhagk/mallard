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
    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;
        tt.link(p, u);
    }

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, x;
            std::cin >> p >> x;

            tt.add(p, x);
        } else {
            u32 u;
            std::cin >> u;

            std::cout << tt.fold_subtree(0, u).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
