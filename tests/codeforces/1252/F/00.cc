#include <bits/stdc++.h>

#include "lib/centroids.h"
#include "lib/pbds.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/rooted_tree_isomorphism.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);

    u32 ans = 0;
    for (u32 u : mld::centroids(g)) {
        mld::pbds::hash_map<u64, u32> m;
        m.resize(n);

        mld::rooted_tree h(g, u);

        std::vector<std::pair<u32, u32>> f;
        f.reserve(2 * n);

        std::vector<u32> c, y;
        c.reserve(g[u].size()), y.reserve(g[u].size());

        for (u32 v : g[u]) {
            f.clear();
            for (u32 k = h.tin[v] + 1; k < h.tin[v] + h.siz[v]; ++k) {
                u32 w = h.tor[k];
                u32 a = h.tin[w] - h.tin[v], b = h.tin[h.par[w]] - h.tin[v];
                f.emplace_back(a, b), f.emplace_back(b, a);
            }

            mld::static_csr z(h.siz[v], f);

            auto d = mld::centroids(z);
            assert(d.size() <= 2);

            if (d.size() == 2) {
                c.push_back(mld::rooted_tree_isomorphism(m, z, d[0])[d[0]]);
                y.push_back(mld::rooted_tree_isomorphism(m, z, d[1])[d[1]]);
            } else {
                c.push_back(mld::rooted_tree_isomorphism(m, z, d[0])[d[0]]);
                y.push_back(c.back());
            }
        }

        assert(c.size() >= 2);

        bool a = true, b = true;
        for (u32 i = 0; i < c.size(); ++i) {
            if (c[0] != c[i] && c[0] != y[i]) a = false;
            if (y[0] != c[i] && y[0] != y[i]) b = false;
        }

        if (a || b) ans = std::max(ans, static_cast<u32>(c.size()));
    }

    std::cout << i32(ans == 0 ? -1 : ans) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
