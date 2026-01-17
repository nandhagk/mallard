#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::am_tree<std::pair<u32, u32>> amt(n);

    u64 ans = 0;
    std::vector<bool> edg(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        if (auto res = amt.insert(u, v, {w, i})) {
            if (auto r = *res) edg[r->second] = false, ans -= r->first;
            edg[i] = true, ans += w;
        }
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
