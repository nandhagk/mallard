#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<u32> c(k);
    for (auto &&x : c) std::cin >> x, --x;

    mld::union_find uf(n);
    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        uf.merge(--u, --v);
    }

    u32 r = c[0];
    for (u32 u : c)
        if (uf.size(u) > uf.size(r)) r = u;

    std::vector<bool> s(n);
    for (u32 u : c) s[uf.find(u)] = 1;
    for (u32 u = 0; u < n; ++u) s[u] = s[uf.find(u)];

    for (u32 u = 0; u < n; ++u)
        if (!s[u]) uf.merge(u, r);

    u32 ans = 0;
    for (u32 u : c) ans += uf.size(u) * (uf.size(u) - 1) / 2;

    assert(ans >= m);
    std::cout << ans - m << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
