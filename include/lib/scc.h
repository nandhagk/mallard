#ifndef MALLARD_SCC_H
#define MALLARD_SCC_H 1

#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename DirectedGraph>
[[nodiscard]] constexpr std::pair<u32, std::vector<u32>>
scc(const DirectedGraph &g) noexcept {
    u32 len = static_cast<u32>(g.size());
    constexpr u32 UNSET = -1;

    std::vector<u32> sen, low(len), tin(len, UNSET), ids(len);
    sen.reserve(len);

    u32 tim = 0, grp = 0;
    auto dfs = [&](auto &&self, u32 u) -> void {
        tin[u] = low[u] = tim++;
        sen.push_back(u);

        for (u32 v : g[u]) {
            if (tin[v] == UNSET) {
                self(self, v);
                low[u] = std::min(low[u], low[v]);
            } else {
                low[u] = std::min(low[u], tin[v]);
            }
        }

        if (u32 v; low[u] == tin[u]) {
            do {
                v = sen.back();
                sen.pop_back();

                tin[v] = len;
                ids[v] = grp;
            } while (u != v);

            ++grp;
        }
    };

    for (u32 u = 0; u < len; ++u)
        if (tin[u] == UNSET) dfs(dfs, u);

    for (auto &&k : ids) k = grp - 1 - k;
    return {grp, std::move(ids)};
}
} // namespace mld

#endif // MALLARD_SCC_H
