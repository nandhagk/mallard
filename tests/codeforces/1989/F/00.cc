#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/incremental_scc.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    mld::offline::incremental_scc isc(n + m);
    isc.reserve(q);

    u64 ans = 0;
    auto f = [&](u32) { std::cout << ans << '\n'; };

    while (q--) {
        u32 x, y;
        char c;
        std::cin >> x >> y >> c;

        --x, --y;
        if (c == 'R')
            isc.link(y + n, x);
        else
            isc.link(x, y + n);

        isc.query(f);
    }

    mld::union_find uf(n + m);
    isc.solve([&](u32 u, u32 v) {
        if (!uf.is_same(u, v)) {
            u64 a = uf.size(u), b = uf.size(v);
            if (a > 1) ans -= a * a;
            if (b > 1) ans -= b * b;
            ans += (a + b) * (a + b);
            uf.merge(u, v);
        }
    });
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
