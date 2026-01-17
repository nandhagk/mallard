#include <bits/stdc++.h>

#include "lib/becc.h"
#include "lib/dynamic_csr.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(4 * m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, i + n);
        e.emplace_back(v, i + n);

        e.emplace_back(i + n, u);
        e.emplace_back(i + n, v);
    }

    mld::static_csr g(n + m, e);
    auto [k, ids] = mld::becc(g);

    mld::dynamic_csr<u32> ccs(k);
    for (u32 u = 0; u < n + m; ++u) ccs[ids[u]].emplace_back(u);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(4 * m);

    for (auto &&[u, v] : e)
        if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);

    mld::static_csr h(k, f);
    mld::rooted_tree r(h);

    std::vector<bool> x(n + m);
    for (u32 cur = ids[n - 1]; cur != ids[0]; cur = r.par[cur]) {
        for (u32 u : ccs[r.par[cur]]) {
            for (u32 v : g[u]) {
                if (ids[v] == cur) {
                    if (u >= n) x[u] = true;
                    goto found;
                }
            }
        }
    found:;
    }

    std::queue<u32> que;
    std::vector<u32> dst(n + m, n + m), par(n + m, n + m);
    for (u32 u = 0; u < n + m; ++u)
        if (x[u]) dst[u] = 0, par[u] = u, que.push(u);

    while (!que.empty()) {
        u32 u = que.front();
        que.pop();

        for (u32 v : g[u])
            if (dst[v] > dst[u] + 1 || (dst[v] == dst[u] + 1 && par[v] > par[u]))
                dst[v] = dst[u] + 1, par[v] = par[u], que.push(v);
    }

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u;
        std::cin >> u;

        --u;
        if (ids[0] == ids[n - 1])
            std::cout << -1 << ' ';
        else
            std::cout << par[u] - n + 1 << ' ';
    }

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
