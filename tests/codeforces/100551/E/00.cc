#include <bits/stdc++.h>

#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;

    mld::am_tree<i32> amt(n);
    mld::offline::dynamic_connectivity<decltype(amt)> dct(n);

    u32 k;
    std::cin >> k;

    dct.reserve(4 * k + m);
    for (auto &&[u, v] : e) dct.link(u, v);

    auto f = [](auto &&am, u32) {
        std::cout << (am.ccs() == 1 ? "Connected\n" : "Disconnected\n");
    };

    while (k--) {
        u32 c;
        std::cin >> c;

        std::vector<u32> a(c);
        for (auto &&x : a) std::cin >> x, --x;

        for (auto &&x : a) {
            auto &&[u, v] = e[x];
            dct.cut(u, v);
        }

        dct.query(f);

        for (auto &&x : a) {
            auto &&[u, v] = e[x];
            dct.link(u, v);
        }
    }

    dct.solve(amt);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifndef NANDHAGK_LOCAL
    std::ifstream in("disconnected.in");
    std::cin.rdbuf(in.rdbuf());

    std::ofstream out("disconnected.out");
    std::cout.rdbuf(out.rdbuf());
#endif

    solve();
}
