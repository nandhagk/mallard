#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

static constexpr u32 N = 200'000;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u32>>> e;
    e.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(w, std::pair<u32, u32>{--u, --v});
    }

    mld::static_csr g(N + 1, e);

    std::vector<u64> ans(N + 1);

    u64 cur = 0;

    mld::union_find uf(n);
    for (u32 w = 0; w <= N; ++w) {
        for (auto &&[u, v] : g[w]) {
            if (!uf.is_same(u, v)) {
                cur += u64(uf.size(u)) * uf.size(v);
                uf.merge(u, v);
            }
        }

        ans[w] = cur;
    }

    while (m--) {
        u32 q;
        std::cin >> q;

        std::cout << ans[q] << ' ';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
