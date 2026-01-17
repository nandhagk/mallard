#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/pbds.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::pbds::hash_set<std::pair<u32, u32>> s;
    s.resize(q);

    mld::offline::dynamic_connectivity<mld::am_tree<i32>> dct(2 * n);
    dct.reserve(q);

    u32 cnt = n;
    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        if (u > v) std::swap(u, v);

        auto [it, res] = s.insert({u, v});
        if (res) {
            dct.query([&cnt, u, v, n](auto &&am, u32) {
                if (am.is_connected(u, v + n)) {
                    assert(am.is_connected(v, u + n));
                    return;
                }

                if (!am.is_connected(u, u + n) || !am.is_connected(v, v + n)) --cnt;
            });

            dct.link(u, v + n);
            dct.link(v, u + n);
        } else {
            s.erase(*it);

            dct.cut(u, v + n);
            dct.cut(v, u + n);

            dct.query([&cnt, u, v, n](auto &&am, u32) {
                if (am.is_connected(u, v + n)) {
                    assert(am.is_connected(v, u + n));
                    return;
                }

                if (!am.is_connected(u, u + n) || !am.is_connected(v, v + n)) ++cnt;
            });
        }

        dct.query([&](auto &&am, u32) {
            std::cout << (am.ccs() == cnt * 2 ? "YES\n" : "NO\n");
        });
    }

    mld::am_tree<i32> amt(2 * n);
    dct.solve(amt);
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
