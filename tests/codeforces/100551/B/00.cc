#include <bits/stdc++.h>

#include "lib/incremental_becc.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::incremental_becc ibc(n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        ibc.merge(--u, --v);
    }

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v;
        std::cin >> u >> v;

        ibc.merge(--u, --v);
        std::cout << ibc.bridges() << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifndef NANDHAGK_LOCAL
    std::ifstream in("bridges.in");
    std::cin.rdbuf(in.rdbuf());

    std::ofstream out("bridges.out");
    std::cout.rdbuf(out.rdbuf());
#endif

    solve();
}
