#include <bits/stdc++.h>

#include "lib/am_tree.h"
#include "lib/fenwick_set.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, k, m;
    std::cin >> n >> k >> m;

    if (m == 0) {
        u32 q;
        std::cin >> q;

        while (q--) {
            u32 u, v;
            std::cin >> u >> v;

            std::cout << v - u + 1 << '\n';
        }

        return;
    }

    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;
    for (auto &&[u, v] : e)
        if (u > v) std::swap(u, v);

    std::ranges::sort(e);

    std::vector<u32> y(n);
    for (u32 u = 0; u < n; ++u) {
        y[u] = static_cast<u32>(std::ranges::lower_bound(e, std::make_pair(u, u)) -
                                e.begin());
    }

    std::vector<std::pair<u32, std::pair<u32, u32>>> z;
    z.reserve(m);
    for (auto &&[u, v] : e) z.emplace_back(v, std::pair<u32, u32>{u, z.size()});

    mld::static_csr h(n, z);

    u32 q;
    std::cin >> q;

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(q);

    for (u32 i = 0; i < q; ++i) {
        u32 l, r;
        std::cin >> l >> r;

        f.emplace_back(--r, std::pair<u32, u32>{--l, i});
    }

    mld::static_csr g(n, f);
    std::vector<u32> ans(q);

    mld::fenwick_set<> fws(m);
    mld::am_tree<i32> amt(n);
    for (u32 v = 0; v < n; ++v) {
        for (auto &&[u, r] : h[v]) {
            if (auto res = amt.insert(u, v, -i32(r))) {
                if (auto x = *res) fws.erase(-(*x));
                fws.insert(r);
            }
        }

        for (auto &&[u, i] : g[v])
            ans[i] = amt.ccs() + fws.rank(y[u]) - (n - (v - u + 1));
    }

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
