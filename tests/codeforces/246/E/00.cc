#include <bits/stdc++.h>

#include "lib/compress.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/static_distinct.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    std::vector<std::string> a;
    a.reserve(n + 1), a.emplace_back();

    for (u32 u = 1; u <= n; ++u) {
        std::string s;
        u32 p;
        std::cin >> s >> p;

        a.emplace_back(s);
        e.emplace_back(p, u);
    }

    auto [z, na] = mld::compress(a);

    mld::static_csr g(n + 1, e);
    mld::hld h(g);

    std::vector<std::pair<u32, u32>> d;
    d.reserve(n + 1);
    for (u32 u = 0; u <= n; ++u) d.emplace_back(h.dep[u], h.tin[u]);
    std::ranges::sort(d);

    mld::static_distinct sd(
        d | std::views::transform([&](auto &&p) { return na[h.tor[p.second]]; }),
        static_cast<u32>(z.size()));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, k;
        std::cin >> u >> k;

        u32 l = static_cast<u32>(
            std::ranges::lower_bound(d, std::make_pair(h.dep[u] + k, h.tin[u])) -
            d.begin());
        u32 r = static_cast<u32>(
            std::ranges::lower_bound(d, std::make_pair(h.dep[u] + k, h.tin[u] + h.siz[u])) -
            d.begin());

        std::cout << (l == r ? 0 : sd.count(l, r)) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
