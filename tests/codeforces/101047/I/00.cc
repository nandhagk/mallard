#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/am_tree.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

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

    while (q--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        if (auto res = amt.insert(--u, --v, w)) {
            if (auto r = *res) ans -= *r;
            ans += w;
        }

        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
