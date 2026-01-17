#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ctd.h"
#include "lib/hld.h"
#include "lib/prelude.h"
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

    if (k == n) {
        std::cout << 0 << '\n';
        return;
    }

    mld::static_csr g(n, e);
    mld::hld h(g);

    constexpr u32 UNSET = -1;
    auto [_, par] = mld::ctd(g);

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, std::greater{},
                      [&](u32 u) { return std::tie(h.dep[u], u); });

    std::vector<u32> dst(n);
    auto f = [&](u32 r) -> u32 {
        std::ranges::fill(dst, n);

        u32 ans = 0;
        for (u32 u : ord) {
            u32 d = dst[u];
            for (u32 v = par[u]; v != UNSET; v = par[v])
                d = std::min(d, dst[v] + h.dist(u, v));

            if (d <= r) continue;

            u32 w = *h.jump(u, std::min(h.dep[u], r));

            dst[w] = 0;
            for (u32 v = par[w]; v != UNSET; v = par[v])
                dst[v] = std::min(dst[v], h.dist(v, w));

            ++ans;
        }

        return ans;
    };

    u32 lo = 0, hi = n;
    while (hi - lo > 1) {
        u32 m = std::midpoint(lo, hi);
        if (f(m) > k)
            lo = m;
        else
            hi = m;
    }

    std::cout << hi << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
