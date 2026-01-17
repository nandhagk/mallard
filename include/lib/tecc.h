#ifndef MALLARD_TECC_H
#define MALLARD_TECC_H 1

#include <vector>

#include "lib/prelude.h"
#include "lib/union_find.h"

namespace mld {
// TODO: Make u32
template <typename UndirectedGraph>
[[nodiscard]] constexpr mld::union_find tecc(const UndirectedGraph &g) noexcept {
    const i32 n = static_cast<i32>(g.size());

    i32 dft = -1;
    std::vector<i32> pre(n, -1), pst(n), pth(n, -1), low(n), deg(n);

    mld::union_find dsu(n);
    auto dfs = [&](auto &&self, i32 u, i32 t = -1) -> void {
        i32 pc{};

        low[u] = pre[u] = ++dft;
        for (i32 v : g[u]) {
            if (v == u || (v == t && !pc++)) continue;

            if (pre[v] != -1) {
                if (pre[v] < pre[u]) {
                    ++deg[u];
                    low[u] = std::min(low[u], pre[v]);
                    continue;
                }

                --deg[u];
                for (i32 &p = pth[u]; p != -1 && pre[p] <= pre[v] && pre[v] <= pst[p];
                     p = pth[p]) {
                    dsu.merge(u, p);
                    deg[u] += deg[p];
                }

                continue;
            }

            self(self, v, u);
            if (pth[v] == -1 && deg[v] <= 1) {
                low[u] = std::min(low[u], low[v]);
                deg[u] += deg[v];
                continue;
            }

            if (deg[v] == 0) v = pth[v];
            if (low[u] > low[v]) {
                low[u] = std::min(low[u], low[v]);
                std::swap(v, pth[u]);
            }

            for (; v != -1; v = pth[v]) {
                dsu.merge(u, v);
                deg[u] += deg[v];
            }
        }

        pst[u] = dft;
    };

    for (i32 u = 0; u < n; ++u)
        if (pre[u] == -1) dfs(dfs, u);

    return dsu;
}
} // namespace mld

#endif // MALLARD_TECC_H
