#include <bits/stdc++.h>

#include "lib/compress.h"
#include "lib/offline/mo_tree.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    auto [w, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 x;
                          std::cin >> x;
                          return x;
                      }));

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

    u32 q;
    std::cin >> q;

    std::vector<u32> y(w.size()), z(w.size());
    std::vector<u64> ans(q);

    mld::offline::mo_tree m(g);
    m.reserve(q);

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        m.query(--u, --v);
    }

    u64 cur = 0;
    m.solve(
        [&](u32 u) {
            cur -= u64(y[na[u]]) * z[na[u]];
            if (a[u])
                ++y[na[u]];
            else
                ++z[na[u]];
            cur += u64(y[na[u]]) * z[na[u]];
        },
        [&](u32 u) {
            cur -= u64(y[na[u]]) * z[na[u]];
            if (a[u])
                --y[na[u]];
            else
                --z[na[u]];
            cur += u64(y[na[u]]) * z[na[u]];
        },
        [&](u32 k, u32, u32) { ans[k] = cur; });

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
