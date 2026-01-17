#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/affine_rev.h"
#include "lib/dynamic_graph.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::affine_rev<Z>;

    std::vector<std::pair<u32, u32>> z(n);
    for (auto &&[a, b] : z) std::cin >> a >> b;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        g.add_edge(a, b);
        g.add_edge(b, a);
    }

    mld::hld_aggregate<mld::segment_tree<operand>> hag(
        g,
        z | std::views::transform([](auto &&x) { return operand(x.first, x.second); }));

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 p, b, c;
            std::cin >> p >> b >> c;

            hag.set(p, operand(b, c));
        } else {
            u32 u, v, x;
            std::cin >> u >> v >> x;

            std::cout << hag.fold_path(u, v)(x).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
