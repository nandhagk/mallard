#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/mo_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> c;
    c.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.emplace_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::vector<u32> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }
    d[0] = n;

    mld::offline::mo_tree m(g);
    m.reserve(q);

    std::vector<u32> ans(q);

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        m.query(--u, --v);
    }

    std::vector<u32> cnt(n);

    u32 blk = static_cast<u32>(std::sqrt(n));
    std::vector<u32> z((n + blk - 1) / blk);
    for (u32 l = 0; l < n; l += blk)
        for (u32 i = l; i < std::min(l + blk, n); ++i) ++z[l / blk];

    m.solve<true>(
        [&](u32 u) {
            if (d[u] >= n) return;
            if (++cnt[d[u]] == 1) --z[d[u] / blk];
        },
        [&](u32 u) {
            if (d[u] >= n) return;
            if (--cnt[d[u]] == 0) ++z[d[u] / blk];
        },
        [&](u32 k, u32, u32) {
            for (u32 u = 0, y = u * blk; y < n; ++u, y += blk) {
                if (!z[u]) continue;
                u32 w = std::min(y + blk, n);
                for (u32 x = y; x < w; ++x) {
                    if (cnt[x] == 0) {
                        ans[k] = x;
                        goto found;
                    }
                }
            }

        found:;
        });

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
