#include <bits/stdc++.h>

#include "lib/algebra/sum.h"
#include "lib/ctd.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(4 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        u32 w = i + n - 1;
        e.emplace_back(u, w);
        e.emplace_back(w, u);

        e.emplace_back(v, w);
        e.emplace_back(w, v);
    }

    mld::static_csr g(2 * n - 1, e);
    mld::hld h(g);

    constexpr u32 UNSET = -1;
    auto [_, par] = mld::ctd(g);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(2 * n * std::bit_width(2 * n));
    for (u32 u = 0; u < 2 * n - 1; ++u)
        for (u32 v = u; v != UNSET; v = par[v]) f.emplace_back(u, h.dist(u, v));

    std::ranges::reverse(f);
    mld::static_csr p(2 * n - 1, f);

    std::vector<u32> c(2 * n), d(2 * n);
    for (u32 u = 0; u < 2 * n - 1; ++u) {
        for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v])
            c[v] = std::max(c[v], p[u][k] + 1);
        for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v])
            d[v] = std::max(d[v], p[u][k]);
    }

    std::exclusive_scan(c.begin(), c.end(), c.begin(), u32{0});
    std::exclusive_scan(d.begin(), d.end(), d.begin(), u32{0});

    using operand = mld::algebra::sum<u32>;
    std::vector<operand> z(c.back()), y(d.back());
    for (u32 u = 0; u < 2 * n - 1; ++u) {
        for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v])
            z[c[v] + p[u][k]] = z[c[v] + p[u][k]] + (u < n);
        for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v])
            y[d[v] + p[u][k] - 1] = y[d[v] + p[u][k] - 1] + (u < n);
    }

    mld::scan_table<operand> sta(z), stb(y);
    auto get = [&](u32 u, u32 w) {
        ++w;

        operand xl, xr;
        for (u32 v = u, k = 0; k < p[u].size(); ++k, v = par[v]) {
            u32 b = p[u][k];
            u32 sl = std::clamp(u32{0}, b, b + c[v + 1] - c[v]);
            u32 sr = std::clamp(w, b, b + c[v + 1] - c[v]);
            if (sl < sr) xr = xr + sta.fold(c[v] + sl - b, c[v] + sr - b);
        }

        for (u32 v = u, k = 1; k < p[u].size(); ++k, v = par[v]) {
            u32 b = p[u][k];
            u32 sl = std::clamp(u32{0}, b + 1, b + d[v + 1] - d[v] + 1);
            u32 sr = std::clamp(w, b + 1, b + d[v + 1] - d[v] + 1);
            if (sl < sr) xl = xl + stb.fold(d[v] + sl - b - 1, d[v] + sr - b - 1);
        }

        return (-xl + xr).val();
    };

    for (u32 lst = 0; q--;) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        u = (u + lst) % n;
        v = (v + lst) % n;
        w = (w + lst) % n, w *= 2;

        u32 t = h.dist(u, v);
        assert(t % 2 == 0);

        lst = get(u, w) + get(v, w);
        if (w >= t / 2) lst -= get(*h.jump(u, v, t / 2), w - (t / 2));

        std::cout << lst << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
