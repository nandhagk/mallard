#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/ctd.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<i32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    auto [_, par] = mld::ctd(g);

    using operand = mld::algebra::sum<i64>;

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(n * std::bit_width(n));

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; v != UNSET; v = par[v])
            f.emplace_back(v, std::pair<u32, u32>{h.dist(u, v), u});

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

    w.clear(), w.shrink_to_fit();
    f.clear(), f.shrink_to_fit();

    mld::fenwick_tree<operand> fwa(z.data() | std::views::elements<1> |
                                   std::views::transform([&](u32 u) { return a[u]; }));
    mld::fenwick_tree<operand> fwb(y.data() | std::views::elements<1> |
                                   std::views::transform([&](u32 u) { return a[u]; }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u;
            i32 x;
            std::cin >> u >> x;

            for (u32 v = u, i = 0; i < p[u].size(); ++i, v = par[v])
                fwa.add(c[v] + p[u][i], x);
            for (u32 v = u, i = 0; i < o[u].size(); ++i, v = par[v])
                fwb.add(d[v] + o[u][i], x);
        } else {
            u32 u, l, r;
            std::cin >> u >> l >> r;

            operand xl, xr;
            for (u32 v = u, i = 0; i < p[u].size(); ++i, v = par[v]) {
                auto &&[b, _] = z[v][p[u][i]];
                u32 sl = static_cast<u32>(
                    std::ranges::lower_bound(z[v], std::max(l, b) - b, {},
                                             &decltype(z[v])::value_type::first) -
                    z[v].begin());
                u32 sr = static_cast<u32>(
                    std::ranges::lower_bound(z[v], std::max(r, b) - b, {},
                                             &decltype(z[v])::value_type::first) -
                    z[v].begin());
                if (sl < sr) xr = xr + fwa.fold(c[v] + sl, c[v] + sr);
            }

            for (u32 v = u, i = 0; i < o[u].size(); ++i, v = par[v]) {
                auto &&[b, _] = y[v][o[u][i]];
                u32 sl = static_cast<u32>(
                    std::ranges::lower_bound(y[v], std::max(l, b) - b, {},
                                             &decltype(y[v])::value_type::first) -
                    y[v].begin());
                u32 sr = static_cast<u32>(
                    std::ranges::lower_bound(y[v], std::max(r, b) - b, {},
                                             &decltype(y[v])::value_type::first) -
                    y[v].begin());
                if (sl < sr) xl = xl + fwb.fold(d[v] + sl, d[v] + sr);
            }

            std::cout << (-xl + xr).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
