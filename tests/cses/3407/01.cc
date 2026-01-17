#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32, u32>> e;
    e.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(w, --u, --v, i);
    }

    std::ranges::sort(e);
    std::vector<bool> ans(m);

    mld::union_find uf(n);

    for (u32 i = 0, j = 0; i < m;) {
        u32 w = std::get<0>(e[i]);
        for (; j < m && std::get<0>(e[j]) == w; ++j)
            ans[std::get<3>(e[j])] = !uf.is_same(std::get<1>(e[j]), std::get<2>(e[j]));

        for (; i < j; ++i) uf.merge(std::get<1>(e[i]), std::get<2>(e[i]));
    }

    for (auto &&x : ans) std::cout << (x ? "YES\n" : "NO\n");
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
