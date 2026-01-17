#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_sum.h"
#include "lib/ctd.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

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

    auto [_, par] = mld::ctd(g);

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(n * std::bit_width(n));

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; v != UNSET; v = par[v])
            f.emplace_back(v, std::pair<u32, u32>{h.dist(u, v), u});

    mld::dual_fenwick_tree<mld::actions::add_sum<u32>> fwa(
        std::views::iota(u32{0}, static_cast<u32>(f.size())) |
        std::views::transform([](auto) { return 0; }));

    mld::static_csr z(n, f);
    for (u32 u = 0; u < n; ++u) std::ranges::sort(z[u]);

    std::vector<u32> s(n);
    for (u32 u = 0; u < n; ++u) s[u] = static_cast<u32>(z[u].size());
    std::exclusive_scan(s.begin(), s.end(), s.begin(), 0);

    f.clear();
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; par[v] != UNSET; v = par[v])
            f.emplace_back(v, std::pair<u32, u32>{h.dist(u, par[v]), u});

    mld::dual_fenwick_tree<mld::actions::add_sum<u32>> fwb(
        std::views::iota(u32{0}, static_cast<u32>(f.size())) |
        std::views::transform([](auto) { return 0; }));

    mld::static_csr y(n, f);
    for (u32 u = 0; u < n; ++u) std::ranges::sort(y[u]);

    std::vector<u32> t(n);
    for (u32 u = 0; u < n; ++u) t[u] = static_cast<u32>(y[u].size());
    std::exclusive_scan(t.begin(), t.end(), t.begin(), 0);

    while (q--) {
        u32 c;
        std::cin >> c;

        if (c == 1) {
            u32 u;
            std::cin >> u;

            u32 ans = 0;
            for (u32 v = --u; v != UNSET; v = par[v]) {
                u32 k = static_cast<u32>(
                    std::ranges::lower_bound(z[v], std::make_pair(h.dist(u, v), u)) -
                    z[v].begin());
                ans += fwa.get(s[v] + k).val();

                if (par[v] != UNSET) {
                    k = static_cast<u32>(
                        std::ranges::lower_bound(y[v],
                                                 std::make_pair(h.dist(u, par[v]), u)) -
                        y[v].begin());
                    ans -= fwb.get(t[v] + k).val();
                }
            }

            std::cout << ans << '\n';
        } else {
            u32 u, d, o;
            std::cin >> u >> o >> d;

            ++d;
            for (u32 v = --u; v != UNSET; v = par[v]) {
                u32 b = h.dist(u, v);
                u32 i = 0;
                u32 j =
                    d >= b ? static_cast<u32>(std::ranges::lower_bound(
                                                  z[v], std::make_pair(d - b, u32{0})) -
                                              z[v].begin())
                           : 0;
                if (i < j) fwa.apply(s[v] + i, s[v] + j, o);

                if (par[v] != UNSET) {
                    b = h.dist(u, par[v]);
                    i = 0;
                    j = d >= b ? static_cast<u32>(
                                     std::ranges::lower_bound(
                                         y[v], std::make_pair(d - b, u32{0})) -
                                     y[v].begin())
                               : 0;
                    if (i < j) fwb.apply(t[v] + i, t[v] + j, o);
                }
            }
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
