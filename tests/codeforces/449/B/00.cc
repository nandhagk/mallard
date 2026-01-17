#include <bits/stdc++.h>

#include "lib/dynamic_csr.h"
#include "lib/prelude.h"
#include "lib/radix_heap.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(2 * m + k + 1);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, std::pair<u32, u64>{v, w});
        e.emplace_back(v, std::pair<u32, u64>{u, w});
    }

    e.emplace_back(0, std::pair<u32, u64>{n, 0});

    for (u32 i = 0; i < k; ++i) {
        u32 v, w;
        std::cin >> v >> w;

        e.emplace_back(n, std::pair<u32, u64>{--v, w});
    }

    mld::static_csr g(n + 1, e);

    std::vector<u64> dst(n + 1, std::numeric_limits<u64>::max());
    mld::radix_heap<u64, u32> q;

    mld::dynamic_csr<u32> prv(n + 1);
    prv.reserve(m + k + 1);

    q.emplace(0, 0);
    dst[0] = 0;

    std::vector<bool> vis(n);
    while (!q.empty()) {
        auto [_, u] = q.top();
        q.pop();

        if (vis[u]) continue;
        vis[u] = true;

        for (auto &&[v, w] : g[u]) {
            u64 d = dst[u] + w;
            if (d < dst[v]) {
                prv[v].clear();
                dst[v] = d;
                prv[v].emplace_back(u);
                q.emplace(d, v);
            } else if (d == dst[v]) {
                prv[v].emplace_back(u);
            }
        }
    }

    u32 ans = k;
    for (u32 u = 1; u < n; ++u) {
        for (u32 v : prv[u])
            if (v != n) goto found;

        --ans;
    found:;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
