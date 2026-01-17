#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> e(m);
    for (auto &&[u, v, w] : e) std::cin >> u >> v >> w, --u, --v;

    u32 ans = std::numeric_limits<u32>::max();

    mld::union_find uf;
    for (u32 i = 32; i--;) {
        u32 z = ans ^ (u32(1) << i);
        uf.build(n);

        for (auto &&[u, v, w] : e)
            if ((w | z) == z) uf.merge(u, v);

        if (uf.ccs() == 1) ans = z;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
