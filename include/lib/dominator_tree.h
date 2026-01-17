#ifndef MALLARD_DOMINATOR_TREE_H
#define MALLARD_DOMINATOR_TREE_H 1

#include <numeric>
#include <string>
#include <vector>

#include "lib/dynamic_graph.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

namespace mld {
template <typename DirectedGraph>
[[nodiscard]] constexpr std::vector<u32>
dominator_tree(const DirectedGraph &g, u32 root = 0, u32 m = 0) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<std::pair<u32, u32>> e;
    if (m != 0) e.reserve(m);

    dynamic_csr<u32> bkt(len);
    bkt.reserve(len);

    std::vector<u32> arr(len, UNSET), par(len, UNSET), rev(len, UNSET),
        sdom(len, UNSET), dom(len, UNSET), dsu(len), lbl(len);

    auto find = [&](auto &&self, u32 u, u32 x = 0) -> u32 {
        if (u == dsu[u]) return x ? UNSET : u;
        u32 v = self(self, dsu[u], x + 1);
        if (v == UNSET) return u;
        if (sdom[lbl[dsu[u]]] < sdom[lbl[u]]) lbl[u] = lbl[dsu[u]];
        dsu[u] = v;
        return x ? v : lbl[u];
    };

    u32 t = 0;
    auto dfs = [&](auto &&self, u32 u) -> void {
        arr[u] = t;
        rev[t] = u;
        lbl[t] = sdom[t] = dsu[t] = t;
        ++t;
        for (u32 w : g[u]) {
            if (arr[w] == UNSET) {
                self(self, w);
                par[arr[w]] = arr[u];
            }

            e.emplace_back(arr[w], arr[u]);
        }
    };

    dfs(dfs, root);
    static_csr h(len, e);

    std::iota(dom.begin(), dom.end(), 0);
    for (u32 i = t; i--;) {
        for (u32 w : h[i]) sdom[i] = std::min(sdom[i], sdom[find(find, w)]);
        if (i != 0) bkt[sdom[i]].emplace_back(i);
        for (u32 w : bkt[i]) {
            u32 v = find(find, w);
            if (sdom[v] == sdom[w])
                dom[w] = sdom[w];
            else
                dom[w] = v;
        }
        if (i > 1) dsu[i] = par[i];
    }

    for (u32 i = 1; i < t; ++i)
        if (dom[i] != sdom[i]) dom[i] = dom[dom[i]];

    std::vector<u32> idom(len, UNSET);
    for (u32 i = 1; i < t; ++i) idom[rev[i]] = rev[dom[i]];

    return idom;
}
} // namespace mld

#endif // MALLARD_DOMINATOR_TREE_H
