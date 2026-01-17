#ifndef MALLARD_CENTROIDS_H
#define MALLARD_CENTROIDS_H 1

#include <algorithm>
#include <ranges>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename Tree>
[[nodiscard]] constexpr std::vector<u32> centroids(const Tree &g) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> par(len, UNSET), tor;

    tor.reserve(len), tor.push_back(0);
    for (u32 i = 0; i < len; ++i) {
        u32 u = tor[i];
        for (u32 v : g[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            tor.push_back(v);
        }
    }

    std::vector<u32> siz(len, 1), bst(len, UNSET);
    for (u32 u : tor | std::views::drop(1) | std::views::reverse) {
        siz[par[u]] += siz[u];
        if (u32 &w = bst[par[u]]; w == UNSET || siz[u] > siz[w]) w = u;
    }

    std::vector<u32> ans;
    ans.reserve(2);

    for (u32 u = 0; u < len; ++u) {
        if (len - siz[u] > len / 2) continue;
        if (bst[u] != UNSET && siz[bst[u]] > len / 2) continue;
        ans.push_back(u);
    }

    return ans;
}
} // namespace mld

#endif // MALLARD_CENTROIDS_H
