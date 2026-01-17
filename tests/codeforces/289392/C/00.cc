#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::offline::dynamic_connectivity<mld::am_tree<i32>> dct(n);

    auto f = [](auto &&uf, u32) { std::cout << uf.ccs() << '\n'; };
    while (m--) {
        char c;
        std::cin >> c;

        if (c == '+') {
            u32 u, v;
            std::cin >> u >> v;

            dct.link(--u, --v);
        } else if (c == '-') {
            u32 u, v;
            std::cin >> u >> v;

            dct.cut(--u, --v);
        } else {
            dct.query(f);
        }
    }

    mld::am_tree<i32> amt(n);
    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
