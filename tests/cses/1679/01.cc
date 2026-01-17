#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"
#include "lib/topological_sort.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    mld::static_csr g(n, e);
    if (auto ord = mld::topological_sort(g))
        for (u32 u : *ord) std::cout << u + 1 << ' ';
    else
        std::cout << "IMPOSSIBLE\n";
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
