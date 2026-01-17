#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/mo_tree.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, --x;

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

    mld::offline::mo_tree m(g);
    m.reserve(q);

    constexpr u32 UNSET = -1;
    std::vector<u32> ans(q, UNSET);

    std::vector<std::pair<u32, u32>> que;
    que.reserve(q);

    while (q--) {
        u32 u, v, l, r;
        std::cin >> u >> v >> l >> r;

        m.query(--u, --v);
        que.emplace_back(--l, r);
    }

    std::vector<u32> cnt(n);

    u32 blk = static_cast<u32>(std::sqrt(n));
    std::vector<u32> z((n + blk - 1) / blk);

    m.solve(
        [&](u32 u) {
            if (cnt[a[u]] ^= 1)
                ++z[a[u] / blk];
            else
                --z[a[u] / blk];
        },
        [&](u32 u) {
            if (cnt[a[u]] ^= 1)
                ++z[a[u] / blk];
            else
                --z[a[u] / blk];
        },
        [&](u32 k, u32, u32) {
            auto &&[l, r] = que[k];
            for (u32 u = l / blk, y = u * blk; y < r; ++u, y += blk) {
                if (!z[u]) continue;
                u32 w = std::min(y + blk, r);
                for (u32 x = std::max(y, l); x < w; ++x) {
                    if (cnt[x]) {
                        ans[k] = x + 1;
                        goto found;
                    }
                }
            }

        found:;
        });

    for (auto &&x : ans) std::cout << i32(x) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
