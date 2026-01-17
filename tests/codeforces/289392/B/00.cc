#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/fenwick_set.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    u32 q;
    std::cin >> q;

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(q);

    for (u32 i = 0; i < q; ++i) {
        u32 l, r;
        std::cin >> l >> r;

        f.emplace_back(--r, std::pair<u32, u32>{--l, i});
    }

    std::vector<u32> ans(q);
    mld::static_csr g(m, f);

    mld::fenwick_set<> fws(m);
    mld::am_tree<i32> amt(n);
    for (u32 r = 0; r < m; ++r) {
        auto &&[u, v] = e[r];
        if (auto res = amt.insert(u, v, -i32(r))) {
            if (auto k = *res) fws.erase(-(*k));
            fws.insert(r);
        }

        for (auto &&[l, i] : g[r]) ans[i] = amt.ccs() + fws.rank(l);
    }

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
