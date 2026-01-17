#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/eppstein.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, s, t, k;
    std::cin >> n >> m >> k >> s >> t;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, std::pair<u32, u64>{--v, w});
    }

    mld::static_csr g(n, e);

    mld::eppstein<mld::managers::clonable<>, decltype(g)> epp(g, --s, --t);
    for (u32 i = 0; i < k; ++i)
        if (auto ans = epp.next())
            std::cout << *ans << '\n';
        else
            std::cout << "NO\n";
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
