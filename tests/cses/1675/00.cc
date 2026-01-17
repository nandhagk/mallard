#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::am_tree<u32> amt(n);

    u64 ans = 0;
    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        if (auto res = amt.insert(--u, --v, w)) {
            if (auto r = *res) ans -= *r;
            ans += w;
        }
    }

    if (amt.ccs() != 1)
        std::cout << "IMPOSSIBLE\n";
    else
        std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
