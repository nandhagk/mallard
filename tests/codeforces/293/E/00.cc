#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ctd.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"
#include "lib/wavelet_matrix.h"

void solve() {
    u32 n, a, b;
    std::cin >> n >> a >> b;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> d(n);
    for (u32 u = 1; u < n; ++u) {
        u32 p, w;
        std::cin >> p >> w;

        d[u] = d[--p] + w;
        e.emplace_back(p, u);
        e.emplace_back(u, p);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    auto dist = [&](u32 u, u32 v) { return d[u] + d[v] - 2 * d[h.lca(u, v)]; };

    auto [_, par] = mld::ctd(g);

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(n * std::bit_width(n));

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; v != UNSET; v = par[v])
            f.emplace_back(v, std::pair<u32, u32>{dist(u, v), u});

    mld::static_csr z(n, f);
    for (u32 u = 0; u < n; ++u) std::ranges::sort(z[u]);

    std::vector<u32> s(n + 1);
    for (u32 u = 0; u < n; ++u) s[u + 1] = static_cast<u32>(z[u].size());
    std::inclusive_scan(s.begin(), s.end(), s.begin());

    std::vector<u32> x;
    x.reserve(f.size());
    for (u32 u = 0; u < n; ++u)
        for (auto &&[_, v] : z[u]) x.push_back(h.dist(u, v));

    f.clear();
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; par[v] != UNSET; v = par[v])
            f.emplace_back(v, std::pair<u32, u32>{dist(u, par[v]), u});

    mld::static_csr y(n, f);
    for (u32 u = 0; u < n; ++u) std::ranges::sort(y[u]);

    std::vector<u32> t(n + 1);
    for (u32 u = 0; u < n; ++u) t[u + 1] = static_cast<u32>(y[u].size());
    std::inclusive_scan(t.begin(), t.end(), t.begin());

    std::vector<u32> w;
    w.reserve(f.size());
    for (u32 u = 0; u < n; ++u)
        for (auto &&[_, v] : y[u]) w.push_back(h.dist(par[u], v));

    mld::wavelet_matrix wma(x, n), wmb(w, n);

    u64 ans = 0;
    for (u32 u = 0; u < n; ++u) {
        for (u32 v = u; v != UNSET; v = par[v]) {
            if (u32 c = dist(u, v), o = h.dist(u, v); b >= c && a >= o) {
                u32 r = static_cast<u32>(
                    std::ranges::upper_bound(z[v], std::make_pair(b - c, n)) -
                    z[v].begin());

                if (r != 0) ans += wma.rank(s[v], s[v] + r, a - o + 1);
            }

            if (par[v] != UNSET) {
                if (u32 c = dist(u, par[v]), o = h.dist(u, par[v]); b >= c && a >= o) {
                    u32 r = static_cast<u32>(
                        std::ranges::upper_bound(y[v], std::make_pair(b - c, n)) -
                        y[v].begin());

                    if (r != 0) ans -= wmb.rank(t[v], t[v] + r, a - o + 1);
                }
            }
        }
    }

    std::cout << (ans - n) / 2 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
