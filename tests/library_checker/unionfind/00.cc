#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::union_find dsu(n);
    while (q--) {
        u32 t, u, v;
        std::cin >> t >> u >> v;

        if (t == 0)
            dsu.merge(u, v);
        else
            std::cout << dsu.is_same(u, v) << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
