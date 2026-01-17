#ifndef MALLARD_BIPARTITION_H
#define MALLARD_BIPARTITION_H 1

#include <optional>
#include <queue>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename BipartiteGraph>
[[nodiscard]] std::optional<std::vector<u32>>
bipartition(const BipartiteGraph &g) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> color(len, UNSET);

    std::queue<u32> q;
    for (u32 u = 0; u < len; ++u) {
        if (color[u] != UNSET) continue;

        q.push(u);
        color[u] = 0;
        while (!q.empty()) {
            u32 v = q.front();
            q.pop();

            for (u32 s : g[v]) {
                if (color[s] == UNSET) {
                    color[s] = color[v] ^ 1;
                    q.push(s);
                } else if (color[s] == color[v]) {
                    return std::nullopt;
                }
            }
        }
    }

    return color;
}
} // namespace mld

#endif // MALLARD_BIPARTITION_H
