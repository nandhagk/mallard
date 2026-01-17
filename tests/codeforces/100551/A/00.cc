#include <bits/stdc++.h>

#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::am_tree<i32> amt(n);
    mld::offline::dynamic_connectivity<decltype(amt)> dct(n);

    auto f = [](auto &&am, u32) { std::cout << am.ccs() << '\n'; };
    while (q--) {
        char t;
        std::cin >> t;

        if (t == '+') {
            u32 u, v;
            std::cin >> u >> v;

            dct.link(--u, --v);
        } else if (t == '-') {
            u32 u, v;
            std::cin >> u >> v;

            dct.cut(--u, --v);
        } else {
            dct.query(f);
        }
    }

    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifndef NANDHAGK_LOCAL
    std::ifstream in("connect.in");
    std::cin.rdbuf(in.rdbuf());

    std::ofstream out("connect.out");
    std::cout.rdbuf(out.rdbuf());
#endif

    solve();
}
