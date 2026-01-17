#ifndef MALLARD_DIJKSTRA_H
#define MALLARD_DIJKSTRA_H 1

#include <cassert>
#include <concepts>
#include <limits>
#include <queue>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/radix_heap.h"

namespace mld {
template <typename T>
using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <typename Graph,
          auto inf = std::numeric_limits<internal::graph_weight_t<Graph>>::max()>
[[nodiscard]] constexpr std::pair<std::vector<internal::graph_weight_t<Graph>>,
                                  std::vector<u32>>
dijkstra(const Graph &g, u32 s) noexcept {
    using weight_type = internal::graph_weight_t<Graph>;

    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    assert(s < len);

    std::vector<weight_type> dst(len, inf);
    std::vector<u32> prv(len, UNSET);

    std::conditional_t<std::unsigned_integral<weight_type>,
                       radix_heap<weight_type, u32>,
                       min_heap<std::pair<weight_type, u32>>>
        q;

    q.emplace(0, s);
    dst[s] = 0;

    std::vector<bool> vis(len);
    while (!q.empty()) {
        auto [_, u] = q.top();
        q.pop();

        if (vis[u]) continue;
        vis[u] = true;

        for (auto &&[v, w] : g[u]) {
            if (auto d = dst[u] + w; d < dst[v]) {
                dst[v] = d, prv[v] = u;
                q.emplace(d, v);
            }
        }
    }

    return {std::move(dst), std::move(prv)};
}
} // namespace mld

#endif // MALLARD_DIJKSTRA_H
