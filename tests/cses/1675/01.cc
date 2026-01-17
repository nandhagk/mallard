#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"
#include "ska_sort.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> y;
    y.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;
        y.emplace_back(w, --u, --v);
    }
    ska::ska_sort(y.begin(), y.end());

    mld::union_find uf(n);

    u64 ans = 0;
    for (auto &&[w, u, v] : y)
        if (uf.merge(u, v)) ans += w;

    if (uf.ccs() != 1)
        std::cout << "IMPOSSIBLE\n";
    else
        std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
