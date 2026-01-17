#include <algorithm>
#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/dag_path_cover.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

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
    auto pth = dag_path_cover(g, static_cast<u32>(e.size()));
    if (pth.size() > 2) {
        std::cout << "NO\n";
        return;
    }

    std::cout << "YES\n";

    for (auto &&p : pth) {
        std::cout << p.size() << ' ';
        for (u32 u : p) std::cout << u + 1 << ' ';
        std::cout << '\n';
    }

    if (pth.size() < 2)
        for (u32 i = 0; i < 2 - pth.size(); ++i) std::cout << "0\n";
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
