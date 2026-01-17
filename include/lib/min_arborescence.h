#ifndef MALLARD_MIN_ARBORESCENCE_H
#define MALLARD_MIN_ARBORESCENCE_H 1

#include <algorithm>
#include <optional>
#include <ranges>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/internal/skew_heap.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"
#include "lib/union_find_rollback.h"

namespace mld {
template <typename T>
[[nodiscard]] constexpr std::optional<std::pair<T, std::vector<u32>>>
min_arborescence(u32 len, const std::vector<std::tuple<u32, u32, T>> &edg,
                 u32 root) noexcept {
    using weight_type = T;
    using heap = internal::skew_heap<weight_type, u32, mld::managers::reusing<>,
                                     std::greater<weight_type>>;

    using handler = heap::handler;
    using pointer = heap::pointer;
    using node = heap::node;

    heap hep;
    constexpr u32 UNSET = -1;

    union_find_rollback ufr(len);
    ufr.reserve(static_cast<u32>(edg.size()));

    std::vector<node> but;
    but.reserve(edg.size());

    std::vector<pointer> buf(len, handler::nil);

    u32 x = 0;
    for (auto &&[u, v, w] : edg) {
        but.push_back(node(w, x));
        buf[v] = hep.meld(buf[v], &but[x++]);
    }

    weight_type ans = 0;
    std::vector<u32> sen(len, UNSET), in(len, UNSET), pth(len), que(len);

    std::vector<std::pair<u32, std::vector<u32>>> cys;
    cys.reserve(len);

    sen[root] = root;
    for (u32 s = 0; s < len; ++s) {
        u32 pos = 0;
        for (u32 u = s; sen[u] == UNSET;) {
            if (buf[u] == handler::nil) return std::nullopt;

            u32 e = buf[u]->val;
            weight_type z = buf[u]->key;

            ans += z;
            buf[u]->apply_at(-z);
            buf[u] = hep.meld(buf[u]->l, buf[u]->r);

            que[pos] = e, pth[pos] = u, pos++;

            sen[u] = s, u = ufr.find(std::get<0>(edg[e]));
            if (sen[u] == s) {
                u32 w, end = pos;
                pointer k = handler::nil;
                do {
                    w = pth[--pos];
                    k = hep.meld(k, buf[w]);
                } while (ufr.merge(u, w));

                u = ufr.find(u), sen[u] = UNSET, buf[u] = k;

                std::vector<u32> cyc(que.begin() + pos, que.begin() + end);
                cys.emplace_back(u, std::move(cyc));
            }
        }

        for (u32 i = 0; i < pos; ++i) in[ufr.find(std::get<1>(edg[que[i]]))] = que[i];
    }

    for (auto &&[u, cyc] : cys | std::views::reverse) {
        ufr.rollback(static_cast<u32>(cyc.size() - 1));

        u32 prv = in[u];
        for (u32 e : cyc) in[ufr.find(std::get<1>(edg[e]))] = e;

        in[ufr.find(std::get<1>(edg[prv]))] = prv;
    }

    std::vector<u32> par(len, UNSET);
    for (u32 u = 0; u < len; ++u)
        if (in[u] != UNSET && u != root) par[u] = std::get<0>(edg[in[u]]);

    return std::make_pair(ans, std::move(par));
}

template <internal::weighted_graph DirectedGraph>
[[nodiscard]] constexpr auto min_arborescence(const DirectedGraph &g, u32 root,
                                              u32 m = 0) noexcept {
    using weight_type = internal::graph_weight_t<DirectedGraph>;

    u32 len = static_cast<u32>(g.size());

    std::vector<std::tuple<u32, u32, weight_type>> edg;
    if (m != 0) edg.reserve(m);

    for (u32 u = 0; u < len; ++u)
        for (auto &&[v, w] : g[u]) edg.emplace_back(u, v, w);

    return min_arborescence<weight_type>(len, edg, root);
}

} // namespace mld

#endif // MALLARD_MIN_ARBORESCENCE_H
