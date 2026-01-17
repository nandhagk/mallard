#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::am_tree<std::pair<u32, u32>> amt(n);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        if (auto res = amt.insert(u, v, {w, i}))
            if (auto r = *res)
                std::cout << r->second << ' ';
            else
                std::cout << -1 << ' ';
        else
            std::cout << i << ' ';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
