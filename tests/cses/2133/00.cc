#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    mld::am_tree<i32> amt(n);
    mld::offline::dynamic_connectivity<decltype(amt)> dct(n);
    dct.reserve(m + k);

    auto f = [](auto &&am, u32) { std::cout << am.ccs() << ' '; };
    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        dct.link(--u, --v);
    }

    dct.query(f);
    for (u32 i = m; i < m + k; ++i) {
        u32 t, u, v;
        std::cin >> t >> u >> v;

        if (t == 1)
            dct.link(--u, --v);
        else
            dct.cut(--u, --v);

        dct.query(f);
    }

    dct.solve(amt);
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
