#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        e.emplace_back(a, b);
        e.emplace_back(b, a);
    }

    mld::static_csr g(n, e);
    mld::hld h(g);
    while (q--) {
        u32 u, v, k;
        std::cin >> u >> v >> k;

        if (auto x = h.jump(u, v, k))
            std::cout << *x << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
