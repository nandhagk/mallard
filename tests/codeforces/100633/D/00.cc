#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/chmax_max.h"
#include "lib/ctd.h"
#include "lib/dual_segment_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

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

    std::vector<u32> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = a[i];
    }

    for (u32 u : h.tor | std::views::drop(1)) d[u] += d[h.par[u]];
    auto dist = [&](u32 u, u32 v) { return d[u] + d[v] - 2 * d[h.lca(u, v)]; };

    auto [_, par] = mld::ctd(g);

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(n * std::bit_width(n));

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u)
        for (u32 v = u; v != UNSET; v = par[v])
            f.emplace_back(v, std::pair<u32, u32>{dist(u, v), u});

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
    f.clear(), f.shrink_to_fit();

    mld::dual_segment_tree<mld::actions::chmax_max<u64>> st(
        std::views::iota(u32{0}, c.back()) |
        std::views::transform([](auto) { return 0; }));

    u32 q;
    std::cin >> q;

    u64 msk = (u64(1) << 32) - 1;
    for (u32 j = 0; j < q; ++j) {
        u32 t, u;
        std::cin >> t >> u;

        --u;
        if (t == 2) {
            u64 ans = 0;
            for (u32 v = u, i = 0; i < p[u].size(); ++i, v = par[v])
                ans = std::max(ans, st.get(c[v] + p[u][i]).val());

            std::cout << (ans & msk) << '\n';
        } else {
            u32 r, x;
            std::cin >> r >> x;

            ++r;
            for (u32 v = u, i = 0; i < p[u].size(); ++i, v = par[v]) {
                auto &&[b, _] = z[v][p[u][i]];
                u32 sr = static_cast<u32>(
                    std::ranges::lower_bound(z[v], std::max(r, b) - b, {},
                                             &decltype(z[v])::value_type::first) -
                    z[v].begin());
                if (0 < sr) st.apply(c[v], c[v] + sr, (u64(j + 1) << 32) | x);
            }
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
