#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/rabin_karp_rev.h"
#include "lib/disjoint_sparse_table.h"
#include "lib/four_russian.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/static_montgomery_modint.h"

using Z1 = mld::montgomerymodint998244353;

using operand = mld::algebra::rabin_karp_rev<Z1>;

template <>
std::tuple<Z1> operand::base = mld::algebra::rabin_karp_rev<Z1>::rand();

void solve() {
    u32 n;
    std::cin >> n;

    std::string s;
    std::cin >> s;

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
    mld::hld_aggregate<mld::four_russian<mld::disjoint_sparse_table<operand>>> hag(
        g, s | std::views::transform([](char c) { return operand(c); }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 u, v, a, b;
        std::cin >> u >> v >> a >> b;

        u32 x = hag.lca(--u, --v), y = hag.lca(--a, --b);
        u32 p = hag.dep[u] + hag.dep[v] - 2 * hag.dep[x];
        u32 t = hag.dep[a] + hag.dep[b] - 2 * hag.dep[y];

        i32 lo = -1, hi = std::min(p, t) + 1;
        while (hi - lo > 1) {
            u32 m = std::midpoint(lo, hi);

            u32 w = m < hag.dep[u] - hag.dep[x] ? *hag.jump(u, m) : *hag.jump(v, p - m);
            u32 c = m < hag.dep[a] - hag.dep[y] ? *hag.jump(a, m) : *hag.jump(b, t - m);

            auto z = hag.fold_path(u, w);
            auto d = hag.fold_path(a, c);

            if (z == d)
                lo = m;
            else
                hi = m;
        }

        std::cout << u32(lo + 1) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
