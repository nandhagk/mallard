#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dominator_tree.h"
#include "lib/dynamic_csr.h"
#include "lib/prelude.h"
#include "lib/radix_heap.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(2 * m);

    mld::dynamic_csr<u32> prv(n);
    prv.reserve(m);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, std::pair<u32, u64>{v, w});
        e.emplace_back(v, std::pair<u32, u64>{u, w});
    }

    mld::static_csr g(n, e);

    std::vector<u64> dst(n, std::numeric_limits<u64>::max());
    mld::radix_heap<u64, u32> q;

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

    for (u32 u = 0; u < n; ++u)
        for (u32 v : prv[u]) f.emplace_back(v, u);

    mld::static_csr h(n, f);
    auto par = mld::dominator_tree(h, 0);

    constexpr u32 UNSET = -1;
    assert(std::ranges::count(par, UNSET) == 1);

    std::vector<std::pair<u32, u32>> z;
    z.reserve(n);

    for (u32 u = 1; u < n; ++u) z.emplace_back(par[u], u);

    mld::static_csr x(n, z);
    mld::rooted_tree r(x);

    for (u32 i = 0; i < e.size(); i += 2) {
        auto &&[u, p] = e[i];
        auto &&[v, _] = p;

        if (par[u] == v) std::swap(u, v);

        if (par[v] == u)
            std::cout << r.siz[v] << '\n';
        else
            std::cout << 0 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
