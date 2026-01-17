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

    std::vector<u32> a;
    a.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        a.push_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    std::vector<u64> dst(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        dst[v] = a[i];
    }

    for (u32 u : h.tor | std::views::drop(1)) dst[u] += dst[h.par[u]];
    auto dist = [&](u32 u, u32 v) { return dst[u] + dst[v] - 2 * dst[h.lca(u, v)]; };

    auto [_, par] = mld::ctd(g);

    std::vector<std::pair<u32, std::pair<u64, u32>>> f;
    f.reserve(n * std::bit_width(n));

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; v != UNSET; v = par[v])
            f.emplace_back(v, std::pair<u64, u32>{dist(u, v), u});

    mld::static_csr z(n, f);

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, {}, [&](u32 u) { return z[u].size(); });

    std::vector<u32> c(n + 1);

    std::vector<std::pair<u32, u32>> s;
    s.reserve(f.size());
    for (u32 v : ord) {
        std::ranges::sort(z[v]);
        c[v] = static_cast<u32>(z[v].size());
        for (u32 k = 0; k < c[v]; ++k) {
            auto &&[_, u] = z[v][k];
            s.emplace_back(u, k);
        }
    }

    std::exclusive_scan(c.begin(), c.end(), c.begin(), u32{0});

    std::ranges::reverse(s);
    mld::static_csr p(n, s);

    s.clear(), s.shrink_to_fit();

    f.clear();
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u, i = 1; i < p[u].size(); ++i, v = par[v])
            f.emplace_back(v, z[par[v]][p[u][i]]);

    mld::static_csr y(n, f);

    std::vector<u32> d(n + 1);

    std::vector<std::pair<u32, u32>> w;
    w.reserve(f.size());
    for (u32 v : ord) {
        std::ranges::sort(y[v]);
        d[v] = static_cast<u32>(y[v].size());
        for (u32 k = 0; k < d[v]; ++k) {
            auto &&[_, u] = y[v][k];
            w.emplace_back(u, k);
        }
    }

    std::exclusive_scan(d.begin(), d.end(), d.begin(), u32{0});

    std::ranges::reverse(w);
    mld::static_csr o(n, w);

    f.clear(), f.shrink_to_fit();

    while (q--) {
        u32 u;
        u64 r;
        std::cin >> u >> r;

        --u, ++r;
        u64 ans = 0;
        for (u32 v = u, i = 0; i < p[u].size(); ++i, v = par[v]) {
            auto &&[b, _] = z[v][p[u][i]];
            u32 sr = static_cast<u32>(
                std::ranges::lower_bound(z[v], std::max(r, b) - b, {},
                                         &decltype(z[v])::value_type::first) -
                z[v].begin());
            if (0 < sr) ans += sr;
        }

        for (u32 v = u, i = 0; i < o[u].size(); ++i, v = par[v]) {
            auto &&[b, _] = y[v][o[u][i]];
            u32 sr = static_cast<u32>(
                std::ranges::lower_bound(y[v], std::max(r, b) - b, {},
                                         &decltype(y[v])::value_type::first) -
                y[v].begin());
            if (0 < sr) ans -= sr;
        }

        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
