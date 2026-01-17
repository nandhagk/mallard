#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ccc.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * (m + n));

    u32 w = n - 1;

    std::vector<bool> s(n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        if (u == 0) --w, s[v] = 1;
        if (v == 0) --w, s[u] = 1;
    }

    mld::static_csr g(n, e);
    auto [z, ids] = mld::ccc(g);

    if (z != 1) {
        std::cout << "impossible\n";
        return;
    }

    for (u32 u = 1; u < n; ++u)
        if (!s[u]) e.emplace_back(0, u), e.emplace_back(u, 0);

    mld::static_csr h(n, e);
    auto [y, _] = mld::ccc(h);

    std::cout << ((k < y - 1 || k > w) ? "impossible\n" : "possible\n");
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
