#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::union_find uf(n);

    u32 ans = 1;
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        uf.merge(--u, --v);
        ans = std::max(ans, uf.size(u));

        std::cout << uf.ccs() << ' ' << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
