#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_sum.h"
#include "lib/algebra/sum.h"
#include "lib/ctd.h"
#include "lib/dual_fenwick_tree.h"
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

    constexpr u32 UNSET = -1;
    auto [_, par] = mld::ctd(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n * std::bit_width(n));
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; v != UNSET; v = par[v]) f.emplace_back(u, h.dist(u, v));

    std::ranges::reverse(f);
    mld::static_csr p(n, f);

    std::vector<u32> c(n + 1), d(n + 1);
    for (u32 u = 0; u < n; ++u) {
        for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v])
            c[v] = std::max(c[v], p[u][k] + 1);
        for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v])
            d[v] = std::max(d[v], p[u][k]);
    }

    std::exclusive_scan(c.begin(), c.end(), c.begin(), u32{0});
    std::exclusive_scan(d.begin(), d.end(), d.begin(), u32{0});

    using action = mld::actions::add_sum<i64>;
    using operand = action::operand;
    mld::dual_fenwick_tree<action> fwa(std::views::iota(u32{0}, c.back()) |
                                       std::views::transform([](auto) { return 0; }));
    mld::dual_fenwick_tree<action> fwb(std::views::iota(u32{0}, d.back()) |
                                       std::views::transform([](auto) { return 0; }));
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 u;
            std::cin >> u;

            operand xl, xr;
            for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v])
                xr = xr + fwa.get(c[v] + p[u][k]);
            for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v])
                xl = xl + fwb.get(d[v] + p[u][k] - 1);

            std::cout << a[u] + (-xl + xr).val() << '\n';
        } else {
            u32 u, l, r;
            i32 x;
            std::cin >> u >> l >> r >> x;

            for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v]) {
                u32 b = p[u][k];
                u32 sl = std::clamp(l, b, b + c[v + 1] - c[v]);
                u32 sr = std::clamp(r, b, b + c[v + 1] - c[v]);
                if (sl < sr) fwa.apply(c[v] + sl - b, c[v] + sr - b, x);
            }

            for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v]) {
                u32 b = p[u][k];
                u32 sl = std::clamp(l, b + 1, b + d[v + 1] - d[v] + 1);
                u32 sr = std::clamp(r, b + 1, b + d[v + 1] - d[v] + 1);
                if (sl < sr) fwb.apply(d[v] + sl - b - 1, d[v] + sr - b - 1, x);
            }
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
