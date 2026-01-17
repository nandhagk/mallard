#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dynamic_csr.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    mld::union_find uf(n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
        uf.merge(u, v);
    }

    mld::static_csr g(n, e);

    mld::dynamic_csr<u32> ccs(n);
    ccs.reserve(n);

    for (u32 u = 0; u < n; ++u) ccs[uf.find(u)].emplace_back(u);

    std::vector<u32> color(n);
    auto dfs = [&](auto &&self, u32 u) -> bool {
        color[u] = 1;
        for (u32 v : g[u]) {
            if (color[v] == 0) {
                if (self(self, v)) return true;
            } else if (color[v] == 1) {
                return true;
            }
        }

        color[u] = 2;
        return false;
    };

    u32 ans = 0;
    for (auto &&cc : ccs) {
        if (cc.empty()) continue;

        bool flg = false;
        for (u32 u : cc)
            if (color[u] == 0) flg |= dfs(dfs, u);

        ans += flg ? uf.size(*cc.begin()) : uf.size(*cc.begin()) - 1;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
