#ifndef MALLARD_TREE_DIAMETER_H
#define MALLARD_TREE_DIAMETER_H 1

#include <algorithm>
#include <type_traits>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <internal::weighted_graph Tree>
[[nodiscard]] constexpr std::tuple<u32, std::vector<u32>,
                                   std::vector<internal::graph_weight_t<Tree>>>
tree_diameter(const Tree &g) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> par(len, UNSET);
    std::vector<internal::graph_weight_t<Tree>> dst(len);

    std::vector<u32> tor;
    tor.reserve(len), tor.push_back(0);

    for (u32 i = 0; i < len; ++i) {
        u32 u = tor[i];
        for (auto &&[v, w] : g[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            dst[v] = dst[u] + w;
            tor.push_back(v);
        }
    }

    u32 s = static_cast<u32>(std::ranges::max_element(dst) - dst.begin());

    std::ranges::fill(par, UNSET);
    tor.clear(), tor.push_back(s);

    dst[s] = 0;
    for (u32 i = 0; i < len; ++i) {
        u32 u = tor[i];
        for (auto &&[v, w] : g[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            dst[v] = dst[u] + w;
            tor.push_back(v);
        }
    }

    return {s, std::move(par), std::move(dst)};
}

template <typename Tree>
    requires(!internal::weighted_graph<Tree>)
[[nodiscard]] constexpr std::tuple<u32, std::vector<u32>, std::vector<u32>>
tree_diameter(const Tree &g) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> par(len, UNSET), dst(len);

    std::vector<u32> tor;
    tor.reserve(len), tor.push_back(0);

    for (u32 i = 0; i < len; ++i) {
        u32 u = tor[i];
        for (u32 v : g[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            dst[v] = dst[u] + 1;
            tor.push_back(v);
        }
    }

    u32 s = static_cast<u32>(std::ranges::max_element(dst) - dst.begin());

    std::ranges::fill(par, UNSET);
    tor.clear(), tor.push_back(s);

    dst[s] = 0;
    for (u32 i = 0; i < len; ++i) {
        u32 u = tor[i];
        for (u32 v : g[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            dst[v] = dst[u] + 1;
            tor.push_back(v);
        }
    }

    return {s, std::move(par), std::move(dst)};
}
} // namespace mld

#endif // MALLARD_TREE_DIAMETER_H
