#include <bits/stdc++.h>

#include "lib/am_tree.h"
#include "lib/fenwick_set.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    mld::static_csr<u32> a(std::views::iota(u32{0}, n) |
                           std::views::transform([m](auto) { return m; }));
    for (u32 i = 0; i < n; ++i)
        for (u32 j = 0; j < m; ++j) std::cin >> a[i][j];

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n * m);

    for (u32 i = 0; i < n; ++i) {
        for (u32 j = 0; j < m; ++j) {
            if (i > 0 && a[i][j] == a[i - 1][j])
                e.emplace_back(j * n + (i - 1), j * n + i);
            if (j > 0 && a[i][j] == a[i][j - 1])
                e.emplace_back((j - 1) * n + i, j * n + i);
        }
    }

    std::ranges::sort(e);

    std::vector<u32> y(n * m);
    for (u32 u = 0; u < n * m; ++u) {
        y[u] = static_cast<u32>(std::ranges::lower_bound(e, std::make_pair(u, u)) -
                                e.begin());
    }

    std::vector<std::pair<u32, std::pair<u32, u32>>> z;
    z.reserve(e.size());
    for (auto &&[u, v] : e) z.emplace_back(v, std::pair<u32, u32>{u, z.size()});

    mld::static_csr h(n * m, z);

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(q);

    for (u32 i = 0; i < q; ++i) {
        u32 l, r;
        std::cin >> l >> r;

        f.emplace_back(r * n - 1, std::pair<u32, u32>{--l * n, i});
    }

    mld::static_csr g(n * m, f);
    std::vector<u32> ans(q);

    mld::fenwick_set<> fws(std::max<u32>(static_cast<u32>(e.size()), 1));
    mld::am_tree<i32> amt(n * m);
    for (u32 v = 0; v < n * m; ++v) {
        for (auto &&[u, r] : h[v]) {
            if (auto res = amt.insert(u, v, -i32(r))) {
                if (auto x = *res) fws.erase(-(*x));
                fws.insert(r);
            }
        }

        for (auto &&[u, i] : g[v])
            ans[i] = amt.ccs() + fws.rank(y[u]) - (n * m - (v - u + 1));
    }

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
