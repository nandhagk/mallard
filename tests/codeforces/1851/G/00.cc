#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> h(n);
    for (auto &&x : h) std::cin >> x;

    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;

    std::ranges::sort(e, {}, [&](auto &&p) {
        auto &&[u, v] = p;
        return std::max(h[u], h[v]);
    });

    u32 q;
    std::cin >> q;

    std::vector<std::tuple<u32, u32, u32, u32>> que;
    que.reserve(q);

    for (u32 i = 0; i < q; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        que.emplace_back(--u, --v, w, i);
    }

    std::ranges::sort(que, {}, [&](auto &&p) {
        auto &&[u, v, w, _] = p;
        return h[u] + w;
    });

    mld::union_find uf(n);
    std::vector<bool> ans(q);

    u32 j = 0;
    for (auto &&[u, v, w, i] : que) {
        for (; j < m; ++j) {
            auto &&[a, b] = e[j];
            if (std::max(h[a], h[b]) > h[u] + w) break;

            uf.merge(a, b);
        }

        ans[i] = uf.is_same(u, v);
    }

    for (auto &&x : ans) std::cout << (x ? "YES\n" : "NO\n");
    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
