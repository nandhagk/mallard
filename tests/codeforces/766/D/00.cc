#include <bits/stdc++.h>

#include "lib/algebra/lxor.h"
#include "lib/potentialized_union_find.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    std::map<std::string, u32> a;
    for (u32 i = 0; i < n; ++i) {
        std::string x;
        std::cin >> x;
        a[x] = i;
    }

    mld::potentialized_union_find<mld::algebra::lxor<bool>> uf(n);
    while (m--) {
        u32 t;
        std::cin >> t;

        std::string x, y;
        std::cin >> x >> y;

        u32 u = a[x], v = a[y];
        if (auto z = uf.potential(u, v)) {
            std::cout << (z->val() == (t == 2) ? "YES\n" : "NO\n");
        } else {
            uf.merge(u, v, t == 2);
            std::cout << "YES\n";
        }
    }

    while (q--) {
        std::string x, y;
        std::cin >> x >> y;

        u32 u = a[x], v = a[y];
        if (auto z = uf.potential(u, v))
            std::cout << (z->val() ? 2 : 1) << '\n';
        else
            std::cout << 3 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
