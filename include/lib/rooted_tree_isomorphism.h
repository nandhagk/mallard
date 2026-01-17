#ifndef MALLARD_ROOTED_TREE_ISOMORPHISM_H
#define MALLARD_ROOTED_TREE_ISOMORPHISM_H 1

#include <algorithm>
#include <map>
#include <ranges>
#include <vector>

#include "lib/hash.h"
#include "lib/prelude.h"

namespace mld {
template <typename Map, typename Tree>
[[nodiscard]] std::vector<u32> rooted_tree_isomorphism(Map &m, const Tree &g,
                                                       u32 root = 0) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> par(len, UNSET), tor;

    tor.reserve(len), tor.push_back(root);
    for (u32 i = 0; i < len; ++i) {
        u32 u = tor[i];
        for (u32 v : g[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            tor.push_back(v);
        }
    }

    std::vector<u32> ans(len);

    hash<u64> h;
    for (u32 u : tor | std::views::reverse) {
        u64 x = 0;
        for (u32 v : g[u])
            if (v != par[u]) x += h(ans[v]);

        ans[u] = m.insert({x, m.size()}).first->second;
    }

    return ans;
}
} // namespace mld

#endif // MALLARD_ROOTED_TREE_ISOMORPHISM_H
