#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, std::pair<u32, u32>>> e;
    e.reserve(n);

    mld::am_tree<i32> amt(n);
    mld::offline::dynamic_connectivity<decltype(amt)> dct(n);
    for (u32 i = 1; i < n; ++i) {
        u32 u, v, x;
        std::cin >> u >> v >> x;

        --x, --u, --v;

        dct.link(u, v);
        e.emplace_back(x, std::pair<u32, u32>{u, v});
    }

    mld::static_csr g(n, e);

    u64 ans = 0;
    for (u32 c = 0; c < n; ++c) {
        for (auto &&[u, v] : g[c]) dct.cut(u, v);
        dct.query([&](auto &&am, u32 x) {
            for (auto &&[u, v] : g[x])
                ans += u64(am.size(u)) * am.size(v);
        });
        for (auto &&[u, v] : g[c]) dct.link(u, v);
    }

    dct.solve(amt);
    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
