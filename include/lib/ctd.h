#ifndef MALLARD_CTD_H
#define MALLARD_CTD_H 1

#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename Tree>
[[nodiscard]] constexpr std::pair<u32, std::vector<u32>> ctd(const Tree &g) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> par(len, UNSET), siz(len), dep(len, len);
    auto dfs = [&](auto &&self, u32 u, u32 t, u32 m, u32 rnk) -> std::pair<u32, u32> {
        u32 x = 1;
        for (u32 v : g[u]) {
            if (v == t || dep[v] < rnk) continue;

            auto [z, c] = self(self, v, u, m, rnk);
            if (z == UNSET) return {UNSET, c};

            x += siz[v] = z;
        }

        if (x * 2 >= m) {
            siz[u] = m;
            dep[u] = rnk;

            for (u32 v : g[u]) {
                if (v == t || dep[v] < rnk) continue;
                auto [z, c] = self(self, v, UNSET, siz[v], rnk + 1);
                par[c] = u;
            }

            if (t != UNSET) {
                auto [z, c] = self(self, t, UNSET, m - x, rnk + 1);
                par[c] = u;
            }

            return {UNSET, u};
        }

        return {x, UNSET};
    };

    auto [_, root] = dfs(dfs, 0, UNSET, len, 0);
    return {root, std::move(par)};
}
} // namespace mld

#endif // MALLARD_CTD_H
