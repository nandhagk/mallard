#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine_rev.h"
#include "lib/ctd.h"
#include "lib/dynamic_modint.h"
#include "lib/hld_aggregate.h"
#include "lib/pbds.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/segment_tree.h"
#include "lib/static_csr.h"

using Z = mld::dynamic_modint_32<-1>;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    Z::set_mod(m);

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    std::vector<u32> c;
    c.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(u, v);
        e.emplace_back(v, u);

        c.emplace_back(w);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    std::vector<u32> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    auto [r, par] = mld::ctd(g);

    using operand = mld::algebra::affine_rev<Z>;
    mld::hld_aggregate<mld::segment_tree<operand>> hag(
        g, d | std::views::transform([](u32 x) { return operand(10, x); }));

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n);

    for (u32 u = 0; u < n; ++u)
        if (u != r) f.emplace_back(par[u], u);

    mld::static_csr z(n, f);
    mld::rooted_tree y(z, r);

    std::vector<Z> pw(n);
    pw[0] = 1;
    Z inv = Z(10).inv();
    for (u32 i = 1; i < n; ++i) pw[i] = pw[i - 1] * inv;

    u64 ans = 0;

    mld::pbds::hash_map<u32, u32> a, b;
    a.resize(2 * n), b.resize(2 * n);
    for (u32 u = 0; u < n; ++u) {
        for (u32 k : z[u]) {
            if (k == y.par[u]) continue;

            for (u32 i = 0; i < y.siz[k]; ++i) {
                u32 v = y.tor[y.tin[k] + i];
                auto [_, x, w] = hag.fold_path(v, u, 1).val();
                ans += b[(-x).val()] + a[(-(w * pw[h.dist(u, v)])).val()];
            }

            for (u32 i = 0; i < y.siz[k]; ++i) {
                u32 v = y.tor[y.tin[k] + i];
                auto [_, x, w] = hag.fold_path(v, u, 1).val();
                ++a[x.val()], ++b[(w * pw[h.dist(u, v)]).val()];
            }
        }

        ans += a[0] + b[0];
        a.clear(), b.clear();
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
