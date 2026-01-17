#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/prod.h"
#include "lib/hld.h"
#include "lib/parallel_union_find.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/static_csr.h"
#include "lib/static_montgomery_modint.h"
#include "lib/union_find.h"

using Z = mld::montgomerymodint1000000007;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        --p;
        e.emplace_back(p, u);
        e.emplace_back(u, p);
    }

    mld::static_csr g(n, e);
    mld::hld h(g);

    mld::union_find uf(n);
    mld::parallel_union_find puf(2 * n);

    std::vector<std::pair<u32, u32>> p(n);
    for (auto &&[l, r] : p) std::cin >> l >> r, --l;

    std::vector<std::pair<u32, u32>> t;
    t.reserve(2 * n);
    for (u32 u : h.tor) t.emplace_back(p[u]);
    for (u32 u : h.tor | std::views::reverse) t.emplace_back(p[u]);

    mld::segment_tree<mld::algebra::prod<Z>> st(t | std::views::transform([](auto &&x) {
                                                    auto &&[l, r] = x;
                                                    return r - l;
                                                }));

    auto f = [&](u32 i, u32 j) {
        auto &&[l1, r1] = t[i];
        auto &&[l2, r2] = t[j];
        st.set(j, 1);

        l1 = std::max(l1, l2), r1 = std::min(r1, r2);
        st.set(i, std::max(r1, l1) - l1);
    };

    for (u32 i = 0; i < n; ++i) puf.merge(i, 2 * n - i - 1, 1, f);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v, a, b;
        std::cin >> u >> v >> a >> b;

        auto x = h.decompose(--u, --v);
        for (auto &&[w, z] : x) {
            w = h.tin[w], z = h.tin[z];
            if (w > z) w = 2 * n - w - 1, z = 2 * n - z - 1;
            ++z;
        }

        auto y = h.decompose(--a, --b);
        for (auto &&[w, z] : y) {
            w = h.tin[w], z = h.tin[z];
            if (w > z) w = 2 * n - w - 1, z = 2 * n - z - 1;
            ++z;
        }

        auto it = x.begin(), ti = y.begin();
        while (it != x.end() && ti != y.end()) {
            auto &&[l1, r1] = *it;
            auto &&[l2, r2] = *ti;
            u32 z = std::min(r1 - l1, r2 - l2);
            puf.merge(l1, l2, z, f);

            l1 += z, l2 += z;
            if (l1 >= r1) ++it;
            if (l2 >= r2) ++ti;
        }

        std::cout << st.fold().val().val() << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
