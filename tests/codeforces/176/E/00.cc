#include <bits/stdc++.h>

#include "lib/algebra/sum.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> c;
    c.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.emplace_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    std::vector<u32> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    mld::hld_aggregate<mld::scan_table<mld::algebra::sum<u64>>> hag(g, d);

    std::set<u32> x;

    u32 q;
    std::cin >> q;

    for (u64 ans = 0; q--;) {
        char t;
        std::cin >> t;

        if (t == '+') {
            u32 u;
            std::cin >> u;

            --u;
            if (!x.empty()) {
                auto it = x.upper_bound(h.tin[u]);

                u32 v, w;
                if (it != x.begin())
                    v = h.tor[*std::prev(it)];
                else
                    v = h.tor[*x.rbegin()];

                if (it != x.end())
                    w = h.tor[*it];
                else
                    w = h.tor[*x.begin()];

                ans -= hag.fold_path(v, w, 1).val();
                ans += hag.fold_path(u, v, 1).val();
                ans += hag.fold_path(u, w, 1).val();
            }

            x.insert(h.tin[u]);
        } else if (t == '-') {
            u32 u;
            std::cin >> u;

            --u;
            x.erase(h.tin[u]);
            if (!x.empty()) {
                auto it = x.upper_bound(h.tin[u]);

                u32 v, w;
                if (it != x.begin())
                    v = h.tor[*std::prev(it)];
                else
                    v = h.tor[*x.rbegin()];

                if (it != x.end())
                    w = h.tor[*it];
                else
                    w = h.tor[*x.begin()];

                ans += hag.fold_path(v, w, 1).val();
                ans -= hag.fold_path(u, v, 1).val();
                ans -= hag.fold_path(u, w, 1).val();
            }
        } else {
            std::cout << ans / 2 << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
