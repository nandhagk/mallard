#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    for (auto &&x : h.siz) std::cout << x - 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
