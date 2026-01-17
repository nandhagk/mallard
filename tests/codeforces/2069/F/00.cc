#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/pbds.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::pbds::hash_set<std::pair<u32, u32>> s, t;
    s.resize(q), t.resize(q);

    mld::offline::dynamic_connectivity<mld::am_tree<i32>> x(n), y(n);
    x.reserve(q), y.reserve(q);

    std::vector<u32> ans(q);
    while (q--) {
        char c;
        u32 u, v;
        std::cin >> c >> u >> v;

        --u, --v;
        if (u > v) std::swap(u, v);

        if (c == 'A') {
            auto [it, res] = s.insert({u, v});
            if (res) {
                x.link(u, v);
            } else {
                x.cut(u, v);
                s.erase(*it);
            }

            if (t.find({u, v}) == t.end()) {
                if (res)
                    y.link(u, v);
                else
                    y.cut(u, v);
            }
        } else {
            auto [it, res] = t.insert({u, v});
            if (!res) t.erase(*it);

            if (s.find({u, v}) == s.end()) {
                if (res)
                    y.link(u, v);
                else
                    y.cut(u, v);
            }
        }

        x.query([&](auto &&am, u32 k) { ans[k] += am.ccs(); });
        y.query([&](auto &&am, u32 k) { ans[k] -= am.ccs(); });
    }

    mld::am_tree<i32> a(n), b(n);
    x.solve(a), y.solve(b);

    for (auto &&z : ans) std::cout << z << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
