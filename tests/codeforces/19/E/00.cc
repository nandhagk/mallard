#include <bits/stdc++.h>

#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;

    mld::offline::dynamic_connectivity<mld::am_tree<i32>> dct(2 * n);
    u32 cnt = n;

    auto link = [&](u32 u, u32 v) {
        dct.query([&cnt, u, v, n](auto &&am, u32) {
            if (am.is_connected(u, v + n)) {
                assert(am.is_connected(v, u + n));
                return;
            }

            if (!am.is_connected(u, u + n) || !am.is_connected(v, v + n)) --cnt;
        });

        dct.link(u, v + n);
        dct.link(v, u + n);
    };

    auto cut = [&](u32 u, u32 v) {
        dct.cut(u, v + n);
        dct.cut(v, u + n);

        dct.query([&cnt, u, v, n](auto &&am, u32) {
            if (am.is_connected(u, v + n)) {
                assert(am.is_connected(v, u + n));
                return;
            }

            if (!am.is_connected(u, u + n) || !am.is_connected(v, v + n)) ++cnt;
        });
    };

    for (auto &&[u, v] : e) link(u, v);

    std::vector<u32> ans;
    ans.reserve(m);

    for (u32 k = 0; k < m; ++k) {
        auto &&[u, v] = e[k];
        cut(u, v);
        dct.query([&, k](auto &&am, u32) {
            if (am.ccs() == cnt * 2) ans.push_back(k);
        });
        link(u, v);
    }

    mld::am_tree<i32> amt(2 * n);
    dct.solve(amt);

    std::cout << ans.size() << '\n';
    for (auto &&x : ans) std::cout << x + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
