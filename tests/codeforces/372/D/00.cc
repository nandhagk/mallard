#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

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
    mld::rooted_tree_lca h(g);

    std::set<u32> x;
    auto f = [&](u32 u) -> u32 {
        if (x.empty()) return 1;

        u32 v, w;
        if (auto it = x.upper_bound(h.tin[u]); it == x.end())
            v = h.tor[*std::prev(it)], w = h.tor[*x.begin()];
        else if (it == x.begin())
            v = h.tor[*x.rbegin()], w = h.tor[*it];
        else
            v = h.tor[*std::prev(it)], w = h.tor[*it];

        return (h.dist(u, v) + h.dist(u, w) - h.dist(v, w)) / 2;
    };

    u32 ans = 0, siz = 0;
    for (u32 l = 0, r = 0; r < n; ++r) {
        siz += f(r);
        x.insert(h.tin[r]);

        for (; siz > k && l <= r; ++l) {
            x.erase(h.tin[l]);
            siz -= f(l);
        }

        ans = std::max(ans, r - l + 1);
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
