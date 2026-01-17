#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/pbds.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_isomorphism.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(p, u);
    }

    mld::static_csr g(n, e);

    mld::pbds::hash_map<u64, u32> m;
    m.resize(n);

    auto ans = mld::rooted_tree_isomorphism(m, g);

    std::cout << m.size() << '\n';
    for (auto &&x : ans) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
