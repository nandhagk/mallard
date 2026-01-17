#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/potentialized_union_find.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::potentialized_union_find<mld::algebra::sum<Z>> puf(n);
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, v, x;
            std::cin >> u >> v >> x;

            if (auto y = puf.potential(u, v)) {
                std::cout << (y->val() == x) << '\n';
            } else {
                std::cout << 1 << '\n';
                puf.merge(u, v, x);
            }
        } else {
            u32 u, v;
            std::cin >> u >> v;

            if (auto x = puf.potential(u, v))
                std::cout << x->val().val() << '\n';
            else
                std::cout << -1 << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
