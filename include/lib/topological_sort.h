#ifndef MALLARD_TOPOLOGICAL_SORT_H
#define MALLARD_TOPOLOGICAL_SORT_H 1

#include <optional>
#include <queue>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename DAG>
[[nodiscard]] std::optional<std::vector<u32>> topological_sort(const DAG &g) noexcept {
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> in(len);
    for (u32 u = 0; u < len; ++u)
        for (u32 v : g[u]) ++in[v];

    std::queue<u32> q;
    for (u32 u = 0; u < len; ++u)
        if (in[u] == 0) q.push(u);

    std::vector<u32> ord;
    ord.reserve(len);

    while (!q.empty()) {
        u32 u = q.front();
        q.pop();

        ord.push_back(u);
        for (u32 v : g[u])
            if (--in[v] == 0) q.push(v);
    }

    if (static_cast<u32>(ord.size()) != len) return std::nullopt;
    return ord;
}
} // namespace mld

#endif // MALLARD_TOPOLOGICAL_SORT_H
