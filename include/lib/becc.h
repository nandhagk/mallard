#ifndef MALLARD_BECC_H
#define MALLARD_BECC_H 1

#include <cassert>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename UndirectedGraph>
[[nodiscard]] constexpr std::pair<u32, std::vector<u32>>
becc(const UndirectedGraph &g) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> dep(len, UNSET), cnt(len), par(len), ids(len), ord;
    ord.reserve(len);

    auto dfs = [&](auto &&self, u32 u) -> void {
        ord.push_back(u);

        for (u32 v : g[u]) {
            if (dep[v] == UNSET) {
                par[v] = u;
                dep[v] = dep[u] + 1;
                self(self, v);
                cnt[u] += cnt[v];
            } else if (dep[v] > dep[u]) {
                --cnt[u];
            } else if (dep[v] < dep[u]) {
                ++cnt[u];
            } else {
                // assert(u == v);
            }
        }

        --cnt[u];
    };

    for (u32 u = 0; u < len; ++u)
        if (dep[u] == UNSET) dep[u] = 0, dfs(dfs, u);

    u32 grp = 0;
    for (u32 u : ord)
        if (dep[u] && cnt[u])
            ids[u] = ids[par[u]];
        else
            ids[u] = grp++;

    return {grp, std::move(ids)};
}
} // namespace mld

#endif // MALLARD_BECC_H
