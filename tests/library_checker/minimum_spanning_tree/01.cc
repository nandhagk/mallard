#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"
#include "ska_sort.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<bool> edg(m);

    std::vector<std::pair<u32, u32>> y, z;
    y.reserve(m), z.reserve(m);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;
        y.emplace_back(u, v);
        z.emplace_back(w, i);
    }
    ska::ska_sort(z.begin(), z.end());

    mld::union_find uf(n);

    u64 ans = 0;
    for (auto &&[w, i] : z) {
        auto &&[u, v] = y[i];
        if (uf.merge(u, v)) ans += w, edg[i] = true;
    }

    std::cout << ans << '\n';
    for (u32 i = 0; i < m; ++i)
        if (edg[i]) std::cout << i << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
