#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/max.h"
#include "lib/dynamic_graph.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

// Remove all constexpr from hld.h
// to make it compile on cses
void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> z(n);
    for (auto &&x : z) std::cin >> x;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        --a, --b;
        g.add_edge(a, b);
        g.add_edge(b, a);
    }

    mld::hld_aggregate<mld::segment_tree<mld::algebra::max<u32>>> hag(g, z);
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 s, x;
            std::cin >> s >> x;

            hag.set(--s, x);
        } else {
            u32 a, b;
            std::cin >> a >> b;

            std::cout << hag.fold_path(--a, --b).val() << ' ';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
