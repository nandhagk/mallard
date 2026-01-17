#ifndef MALLARD_SPFA_H
#define MALLARD_SPFA_H 1

#include <concepts>
#include <limits>
#include <utility>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <typename Graph>
using graph_weight_t =
    std::remove_cvref_t<decltype(std::get<1>(*std::declval<Graph>()[0].begin()))>;

template <typename Graph,
          auto inf = std::numeric_limits<internal::graph_weight_t<Graph>>::max(),
          auto minf = std::numeric_limits<internal::graph_weight_t<Graph>>::min()>
[[nodiscard]]
std::pair<std::vector<internal::graph_weight_t<Graph>>, std::vector<u32>>
spfa(const Graph &g, u32 s) noexcept {
    using weight_type = internal::graph_weight_t<Graph>;

    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    assert(s < len);

    std::vector<weight_type> dst(len, inf);
    std::vector<u32> prv(len, UNSET), cnt(len);
    std::deque<u32> que;
    std::vector<bool> inq(len), neg(len);

    dst[s] = 0, que.push_back(s), inq[s] = 1;
    while (!que.empty()) {
        u32 u = que.front();
        que.pop_front(), inq[u] = 0;
        for (auto &&[v, w] : g[u]) {
            if (dst[v] > dst[u] + w) {
                dst[v] = dst[u] + w, prv[v] = u;
                if (!inq[v]) {
                    inq[v] = 1;
                    if (++cnt[v] > len)
                        neg[v] = 1;
                    else
                        que.push_back(v);
                }
            }
        }
    }

    for (u32 u = 0; u < len; ++u)
        if (neg[u]) que.push_back(u);

    while (!que.empty()) {
        u32 u = que.front();
        que.pop_front();

        for (auto &&[v, _] : g[u])
            if (!neg[v]) neg[v] = 1, que.push_back(v);
    }

    for (u32 u = 0; u < len; ++u)
        if (neg[u]) dst[u] = minf;

    return {std::move(dst), std::move(prv)};
}
} // namespace mld

#endif // MALLARD_SPFA_H
