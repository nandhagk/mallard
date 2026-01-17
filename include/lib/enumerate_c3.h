#ifndef MALLARD_ENUMERATE_C3_H
#define MALLARD_ENUMERATE_C3_H 1

#include <numeric>
#include <vector>

#include "lib/prelude.h"
#include "lib/static_csr.h"

namespace mld {
template <typename UndirectedGraph, std::invocable<u32, u32, u32> F>
constexpr void enumerate_c3(const UndirectedGraph &g, F &&f) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> deg(len);
    for (u32 u = 0; u < len; ++u) deg[u] = static_cast<u32>(g[u].size());

    std::vector<std::pair<u32, u32>> e;
    e.reserve(std::reduce(deg.begin(), deg.end()));

    for (u32 u = 0; u < len; ++u) {
        for (u32 v : g[u]) {
            if (u >= v) continue;

            if (deg[u] <= deg[v])
                e.emplace_back(v, u);
            else
                e.emplace_back(u, v);
        }
    }

    static_csr h(len, e);
    std::vector<u32> x(len, UNSET);
    for (u32 u = 0; u < len; ++u) {
        for (u32 v : h[u]) x[v] = u;

        for (u32 v : h[u]) {
            for (u32 w : h[v])
                if (x[w] == u) f(u, v, w);
        }
    }
}
} // namespace mld

#endif // MALLARD_ENUMERATE_C3_H
