#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/ctd.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, l, r;
    std::cin >> n >> l;
    r = l + 1;

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

    using operand = mld::algebra::sum<u64>;
    std::vector<u32> z(c.back()), y(d.back());
    for (u32 u = 0; u < n; ++u) {
        for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v])
            z[c[v] + p[u][k]] = z[c[v] + p[u][k]] + 1;
        for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v])
            y[d[v] + p[u][k] - 1] = y[d[v] + p[u][k] - 1] + 1;
    }

    mld::scan_table<operand> sca(z), scb(y);

    u64 ans = 0;
    for (u32 u = 0; u < n; ++u) {
        operand xl, xr;
        for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v]) {
            u32 b = p[u][k];
            u32 sl = std::clamp(l, b, b + c[v + 1] - c[v]);
            u32 sr = std::clamp(r, b, b + c[v + 1] - c[v]);
            if (sl < sr) xr = xr + sca.fold(c[v] + sl - b, c[v] + sr - b);
        }

        for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v]) {
            u32 b = p[u][k];
            u32 sl = std::clamp(l, b + 1, b + d[v + 1] - d[v] + 1);
            u32 sr = std::clamp(r, b + 1, b + d[v + 1] - d[v] + 1);
            if (sl < sr) xl = xl + scb.fold(d[v] + sl - b - 1, d[v] + sr - b - 1);
        }

        ans += (-xl + xr).val();
    }

    std::cout << ans / 2 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
