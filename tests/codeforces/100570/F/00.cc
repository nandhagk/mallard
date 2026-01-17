#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ctd.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

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

        c.push_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    std::vector<u64> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    for (u32 u : h.tor | std::views::drop(1)) d[u] += d[h.par[u]];
    auto dist = [&](u32 u, u32 v) { return d[u] + d[v] - 2 * d[h.lca(u, v)]; };

    auto [r, par] = mld::ctd(g);

    std::vector<std::pair<u32, u64>> f;
    f.reserve(n * std::bit_width(n));

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; v != UNSET; v = par[v]) f.emplace_back(v, dist(u, v));

    mld::static_csr z(n, f);
    for (u32 u = 0; u < n; ++u) std::ranges::sort(z[u]);

    f.clear();
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; par[v] != UNSET; v = par[v]) f.emplace_back(v, dist(u, par[v]));

    mld::static_csr y(n, f);
    for (u32 u = 0; u < n; ++u) std::ranges::sort(y[u]);

    while (q--) {
        u32 u;
        u64 l;
        std::cin >> u >> l;

        u32 ans = 0;
        for (u32 v = --u; v != UNSET; v = par[v]) {
            if (u64 x = dist(u, v); l >= x) {
                ans += static_cast<u32>(std::ranges::upper_bound(z[v], l - x) -
                                        z[v].begin());
            }

            if (par[v] != UNSET) {
                if (u64 x = dist(u, par[v]); l >= x) {
                    ans -= static_cast<u32>(std::ranges::upper_bound(y[v], l - x) -
                                            y[v].begin());
                }
            }
        }

        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
