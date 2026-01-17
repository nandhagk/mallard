#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/pbds.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_isomorphism.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e, f;
    e.reserve(2 * n), f.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        f.emplace_back(u, v);
        f.emplace_back(v, u);
    }

    mld::static_csr g(n, e), h(n, f);

    mld::pbds::hash_map<u64, u32> m;
    m.resize(2 * n);

    auto x = mld::rooted_tree_isomorphism(m, g);
    auto y = mld::rooted_tree_isomorphism(m, h);

    std::cout << (x[0] == y[0] ? "YES\n" : "NO\n");
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
