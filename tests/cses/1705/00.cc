#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/bcc.h"
#include "lib/block_cut_tree.h"
#include "lib/dynamic_graph.h"
#include "lib/rooted_tree_lca.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    mld::dynamic_graph<u32> g(n);
    g.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g.add_edge(u, v);
        g.add_edge(v, u);
    }

    auto [cut, ccs] = mld::bcc(g);
    auto [k, ids, bct] = mld::block_cut_tree(g, cut, ccs);

    mld::rooted_tree_lca h(bct);
    while (q--) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v, --w;
        if (u == w || v == w || (cut[w] && h.is_on_path(ids[u], ids[v], ids[w])))
            std::cout << "NO\n";
        else
            std::cout << "YES\n";
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
