#ifndef MALLARD_BIPOLAR_ORDER_H
#define MALLARD_BIPOLAR_ORDER_H 1

#include <algorithm>
#include <cassert>
#include <optional>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename UndirectedGraph>
[[nodiscard]] constexpr std::optional<std::vector<u32>>
bipolar_order(const UndirectedGraph &g, u32 s, u32 t) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    if (len == 1) return std::vector<u32>{0};
    if (s == t) return std::nullopt;

    std::vector<u32> par(len, UNSET), tin(len, UNSET), low(len, UNSET);

    std::vector<u32> tour;
    tour.reserve(len);

    auto dfs = [&](auto &&self, u32 u) -> void {
        tin[u] = static_cast<u32>(tour.size());
        tour.push_back(u);

        low[u] = u;
        for (u32 v : g[u]) {
            if (u == v) continue;
            if (tin[v] == UNSET) {
                self(self, v);
                par[v] = u;
                if (tin[low[v]] < tin[low[u]]) low[u] = low[v];
            } else if (tin[v] < tin[low[u]]) {
                low[u] = v;
            }
        }
    };

    tin[s] = 0;
    tour.push_back(s);

    dfs(dfs, t);
    if (static_cast<u32>(tour.size()) != len) return std::nullopt;

    std::vector<u32> nxt(len, UNSET), prv(len);
    nxt[s] = t;
    prv[t] = s;

    std::vector<u32> sgn(len);
    sgn[s] = UNSET;

    for (u32 i = 2; i < len; ++i) {
        u32 u = tour[i];
        u32 p = par[u];

        if (sgn[low[u]] == UNSET) {
            u32 q = prv[p];
            if (q == UNSET) return std::nullopt;

            nxt[q] = u, nxt[u] = p;
            prv[u] = q, prv[p] = u;
            sgn[p] = 1;
        } else {
            u32 q = nxt[p];
            if (q == UNSET) return std::nullopt;

            nxt[p] = u, nxt[u] = q;
            prv[u] = p, prv[q] = u;
            sgn[p] = UNSET;
        }
    }

    std::vector<u32> a;

    a.reserve(len);
    for (a.push_back(s); a.back() != t; a.push_back(nxt[a.back()]));

    if (static_cast<u32>(a.size()) < len) return std::nullopt;
    assert(a[0] == s && a.back() == t);

    std::vector<u32> rnk(len, UNSET);
    for (u32 i = 0; i < len; ++i) rnk[a[i]] = i;

    for (u32 i = 0; i < len; ++i) {
        bool l = false, r = false;
        u32 u = a[i];

        for (u32 v : g[u]) {
            if (rnk[v] < rnk[u]) l = true;
            if (rnk[u] < rnk[v]) r = true;
        }

        if (i > 0 && !l) return std::nullopt;
        if (i < len - 1 && !r) return std::nullopt;
    }

    return rnk;
}
} // namespace mld

#endif // MALLARD_BIPOLAR_ORDER_H
