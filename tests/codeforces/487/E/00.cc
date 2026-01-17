#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/bcc.h"
#include "lib/block_cut_tree.h"
#include "lib/erasable_heap.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/segment_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);

    auto [cut, ccs] = mld::bcc(g);
    auto [k, ids, bct] = mld::block_cut_tree(g, cut, ccs);

    u32 cnt = std::reduce(cut.begin(), cut.end(), u32{0});

    std::vector<mld::erasable_heap<std::pair<u32, u32>, std::greater<>>> hep(k);
    for (u32 u = 0; u < n; ++u) hep[ids[u]].insert(a[u], u);

    mld::rooted_tree h(bct);

    constexpr u32 UNSET = -1;
    for (u32 u = 0; u < n; ++u)
        if (cut[u] && h.par[ids[u]] != UNSET) hep[h.par[ids[u]]].insert(a[u], u);

    mld::hld_aggregate<mld::segment_tree<mld::algebra::min<u32>>> hag(
        bct, std::views::iota(u32{0}, k) |
                 std::views::transform([&](u32 u) { return hep[u].top().first; }));

    while (q--) {
        char t;
        std::cin >> t;

        if (t == 'C') {
            u32 u, w;
            std::cin >> u >> w;

            u32 v = ids[--u];
            if (v < cnt && h.par[v] != UNSET) hep[h.par[v]].erase(a[u], u);
            hep[v].erase(a[u], u);

            a[u] = w;

            if (v < cnt && h.par[v] != UNSET) {
                hep[h.par[v]].insert(a[u], u);
                hag.set(h.par[v], hep[h.par[v]].top().first);
            }

            hep[v].insert(a[u], u);
            hag.set(v, hep[v].top().first);
        } else {
            u32 u, v;
            std::cin >> u >> v;

            --u, --v;
            if (u == v) {
                std::cout << a[u] << '\n';
                continue;
            }

            u = ids[u], v = ids[v];
            auto z = hag.fold_path(u, v);
            if (u32 w = hag.lca(u, v); w >= cnt && h.par[w] != UNSET)
                z = z + hag.get(h.par[w]);

            std::cout << z.val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
