#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/lxor.h"
#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/potentialized_union_find.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;

    {
        mld::potentialized_union_find<mld::algebra::lxor<bool>> puf(n);
        for (auto &&[u, v] : e) {
            if (puf.is_same(u, v)) {
                if (puf.potential(u, v)->val() == 0) goto invalid;
            } else {
                puf.merge(u, v, 1);
            }
        }

        std::cout << "YES\n";
        for (u32 u = 0; u < n; ++u) std::cout << puf.potential(u).val();
        std::cout << '\n';

        return;

    invalid:;
    }

    mld::offline::dynamic_connectivity<mld::am_tree<i32>> dct(2 * n);
    dct.reserve(4 * m);

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

    if (ans.empty()) {
        std::cout << "NO\n";
        return;
    }

    mld::potentialized_union_find<mld::algebra::lxor<bool>> puf(n);
    for (u32 k = 0; k < m; ++k) {
        if (k == ans[0]) continue;
        auto &&[u, v] = e[k];
        puf.merge(u, v, 1);
    }

    std::vector<u32> col(n);
    for (u32 u = 0; u < n; ++u) col[u] = puf.potential(u).val();

    auto &&[u, v] = e[ans[0]];

    debug(u, v);
    if (puf.is_same(u, v)) {
        if (!col[u])
            for (u32 w = 0; w < n; ++w) col[w] ^= puf.is_same(w, u);
    } else { //
        if (!col[u])
            for (u32 w = 0; w < n; ++w) col[w] ^= puf.is_same(w, u);
        if (!col[v])
            for (u32 w = 0; w < n; ++w) col[w] ^= puf.is_same(w, v);
    }

    std::cout << "YES\n";
    for (u32 w = 0; w < n; ++w) std::cout << col[w];
    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
