#ifndef MALLARD_BELLMAN_FORD_H
#define MALLARD_BELLMAN_FORD_H 1

#include <concepts>
#include <limits>
#include <utility>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <typename Graph,
          auto inf = std::numeric_limits<internal::graph_weight_t<Graph>>::max(),
          auto minf = std::numeric_limits<internal::graph_weight_t<Graph>>::min()>
[[nodiscard]] std::pair<std::vector<internal::graph_weight_t<Graph>>, std::vector<u32>>
bellman_ford(const Graph &g, u32 s) noexcept {
    using weight_type = internal::graph_weight_t<Graph>;

    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    assert(s < len);

    std::vector<weight_type> dst(len, inf);
    std::vector<u32> prv(len, UNSET);

    dst[s] = 0;
    for (u32 i = 0; i < len - 1; ++i) {
        bool upd = 0;
        for (u32 u = 0; u < len; ++u) {
            for (auto &&[v, w] : g[u]) {
                if (dst[u] == inf) continue;
                if (dst[u] + w < dst[v]) dst[v] = dst[u] + w, prv[v] = u, upd = 1;
            }
        }

        if (!upd) return {std::move(dst), std::move(prv)};
    }

    std::vector<bool> neg(len);
    for (u32 u = 0; u < len; ++u) {
        for (auto &&[v, w] : g[u]) {
            if (dst[u] == inf) continue;
            if (dst[u] + w < dst[v]) dst[v] = dst[u] + w, prv[v] = u, neg[v] = 1;
        }
    }

    std::queue<u32> que;
    for (u32 u = 0; u < len; ++u)
        if (neg[u]) que.push(u);

    while (!que.empty()) {
        u32 u = que.front();
        que.pop();

        for (auto &&[v, _] : g[u])
            if (!neg[v]) neg[v] = 1, que.push(v);
    }

    for (u32 u = 0; u < len; ++u)
        if (neg[u]) dst[u] = minf;

    return {std::move(dst), std::move(prv)};
}
} // namespace mld

#endif // MALLARD_BELLMAN_FORD_H
