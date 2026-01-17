#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::union_find uf(n);

    constexpr u32 UNSET = -1;
    while (m--) {
        u32 k;
        std::cin >> k;

        for (u32 p = UNSET; k--;) {
            u32 u;
            std::cin >> u;

            --u;
            if (p != UNSET) uf.merge(p, u);

            p = u;
        }
    }

    for (u32 u = 0; u < n; ++u) std::cout << uf.size(u) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
