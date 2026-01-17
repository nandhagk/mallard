#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/suffix_array_query.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::string s;
    std::cin >> s;

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
    mld::hld h(g);

    std::vector<u32> t;
    t.reserve(2 * n);
    for (u32 u : h.tor) t.push_back(s[u] - 'a');
    std::ranges::reverse_copy(t, std::back_insert_iterator(t));

    mld::suffix_array_query saq(t, 26);

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

        u32 ans = 0;

        auto it = x.begin(), ti = y.begin();
        while (it != x.end() && ti != y.end()) {
            auto &&[l1, r1] = *it;
            auto &&[l2, r2] = *ti;
            u32 z = std::min(r1 - l1, r2 - l2);

            u32 k = saq.lcp(l1, l2);
            ans += std::min(k, z);

            if (k < z) break;

            l1 += z, l2 += z;
            if (l1 >= r1) ++it;
            if (l2 >= r2) ++ti;
        }

        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
