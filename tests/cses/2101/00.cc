#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    mld::am_tree<u32> amt(n);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        amt.insert(--u, --v, i);
    }

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        if (u == v)
            std::cout << 0 << '\n';
        else if (auto x = amt.max_path(--u, --v))
            std::cout << *x + 1 << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
