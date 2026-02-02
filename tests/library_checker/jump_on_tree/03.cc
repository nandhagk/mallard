#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/four_russian.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/static_csr.h"
#include "lib/algebra/min.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        e.emplace_back(a, b);
        e.emplace_back(b, a);
    }

    mld::static_csr g(n, e);
    mld::hld h(g);

    mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>> agg(
        h.tor | std::views::drop(1) |
        std::views::transform([&](u32 u) { return h.tin[h.par[u]]; }));

    auto lca = [&](u32 u, u32 v) {
        if (u == v) return u;

        std::tie(u, v) = std::minmax(h.tin[u], h.tin[v]);
        return h.tor[agg.fold(u, v).val()];
    };

    auto jump = [&](u32 u, u32 v, u32 k) -> std::optional<u32> {
        if (u32 w = lca(u, v), l = h.dep[u] - h.dep[w], r = h.dep[v] - h.dep[w];
            l + r < k) {
            return std::nullopt;
        } else if (k < l) {
            return h.jump(u, k);
        } else {
            return h.jump(v, l + r - k);
        }
    };

    while (q--) {
        u32 u, v, k;
        std::cin >> u >> v >> k;

        if (auto x = jump(u, v, k))
            std::cout << *x << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
