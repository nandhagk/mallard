#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    mld::union_find uf(n);
    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        if (!uf.merge(--u, --v)) e.emplace_back(u, v);
    }

    if (uf.ccs() == 1) {
        std::cout << 0 << '\n';
        return;
    }

    std::vector<u32> a(n);
    for (u32 u = 0; u < n; ++u) a[u] = uf.find(u);

    std::ranges::sort(a);
    a.erase(std::unique(a.begin(), a.end()), a.end());

    std::cout << uf.ccs() - 1 << '\n';

    assert(e.size() == uf.ccs() - 1);
    for (u32 i = 0; i < e.size(); ++i) {
        auto &&[u, v] = e[i];
        std::cout << u + 1 << ' ' << v + 1 << ' ' << a[0] + 1 << ' ' << a[i + 1] + 1
                  << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
