#ifndef MALLARD_EPPSTEIN_H
#define MALLARD_EPPSTEIN_H 1

#include <limits>
#include <optional>

#include "lib/dijkstra.h"
#include "lib/internal/concepts.h"
#include "lib/internal/leftist_heap.h"
#include "lib/prelude.h"
#include "lib/radix_heap.h"
#include "lib/static_csr.h"

namespace mld {
// TODO: add path recovery https://judge.yosupo.jp/submission/260946
template <typename Manager, typename Graph,
          auto inf = std::numeric_limits<internal::graph_weight_t<Graph>>::max()>
struct eppstein {
    using weight_type = internal::graph_weight_t<Graph>;
    using size_type = u32;

private:
    using heap = internal::leftist_heap<weight_type, size_type, Manager,
                                        std::greater<weight_type>>;
    heap lhp;

    using handler = heap::handler;
    using pointer = heap::pointer;

    std::conditional_t<std::unsigned_integral<weight_type>,
                       radix_heap<weight_type, pointer>,
                       min_heap<std::pair<weight_type, pointer>>>
        hep;

    std::vector<pointer> buf;

public:
    using allocator_type = heap::allocator_type;

    eppstein(const Graph &g, size_type s, size_type t,
             const allocator_type &alloc = allocator_type()) noexcept
        : lhp(alloc) {
        constexpr size_type UNSET = -1;
        size_type len = static_cast<size_type>(g.size());

        std::vector<std::pair<size_type, std::pair<size_type, weight_type>>> e;
        for (size_type u = 0; u < len; ++u)
            for (auto &&[v, w] : g[u])
                e.emplace_back(v, std::pair<size_type, weight_type>{u, w});

        static_csr z(len, e);

        auto [dst, prv] = mld::dijkstra<decltype(z), inf>(z, t);
        if (dst[s] == inf) return;

        std::vector<std::pair<size_type, size_type>> f;
        f.reserve(len);

        for (size_type u = 0; u < len; ++u)
            if (prv[u] != UNSET) f.emplace_back(prv[u], u);

        static_csr x(len, f);

        std::queue<size_type> q;
        q.push(t);

        buf.resize(len, handler::nil);
        while (!q.empty()) {
            size_type u = q.front();
            q.pop();

            bool sen = false;
            for (auto &&[v, w] : g[u]) {
                if (dst[v] == inf) continue;

                auto c = w + dst[v] - dst[u];
                if (!sen && v == prv[u] && c == 0) {
                    sen = true;
                    continue;
                }

                lhp.insert(buf[u], c, v);
            }

            for (size_type v : x[u]) buf[v] = buf[u], q.push(v);
        }

        hep.emplace(dst[s], handler::nil);
        if (buf[s] != handler::nil) hep.emplace(dst[s] + buf[s]->key, buf[s]);
    }

    std::optional<weight_type> next() noexcept {
        if (hep.empty()) return std::nullopt;

        auto [d, u] = hep.top();
        hep.pop();

        if (u == handler::nil) return d;

        if (buf[u->val] != handler::nil) hep.emplace(d + buf[u->val]->key, buf[u->val]);
        if (u->l != handler::nil) hep.emplace(d + u->l->key - u->key, u->l);
        if (u->r != handler::nil) hep.emplace(d + u->r->key - u->key, u->r);
        return d;
    }
};
} // namespace mld

#endif // MALLARD_EPPSTEIN_H
