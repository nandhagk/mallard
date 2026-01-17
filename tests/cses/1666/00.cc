#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    mld::union_find uf(n);
    while (k--) {
        u32 u, v;
        std::cin >> u >> v;

        uf.merge(--u, --v);
    }

    std::cout << uf.ccs() - 1 << '\n';
    for (u32 u = 1; u < n; ++u)
        if (uf.merge(u - 1, u)) std::cout << u << ' ' << u + 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
