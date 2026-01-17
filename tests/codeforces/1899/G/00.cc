#include <bits/stdc++.h>

#include "lib/fenwick_set.h"
#include "lib/offline/mo.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::vector<u32> p(n);
    for (auto &&x : p) std::cin >> x, --x;

    std::vector<u32> z(n);
    for (u32 i = 0; i < n; ++i) z[p[i]] = i;

    mld::offline::mo m(n);
    m.reserve(q);

    std::vector<u32> l, r;
    l.reserve(q), r.reserve(q);

    std::vector<bool> ans(q);
    while (q--) {
        u32 a, b, x;
        std::cin >> a >> b >> x;

        --a, --x;
        l.emplace_back(a), r.emplace_back(b);
        m.query(h.tin[x], h.tin[x] + h.siz[x]);
    }

    mld::fenwick_set<> fws(n);

    m.solve([&](u32 i) { fws.insert(z[h.tor[i]]); },
            [&](u32 i) { fws.erase(z[h.tor[i]]); },
            [&](u32 i) { ans[i] = fws.rank(l[i], r[i]); });

    for (auto &&x : ans) std::cout << (x ? "YES\n" : "NO\n");
    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
