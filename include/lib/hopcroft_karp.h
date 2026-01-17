#ifndef MALLARD_HOPCROFT_KARP_H
#define MALLARD_HOPCROFT_KARP_H 1

#include <limits>
#include <queue>
#include <vector>

#include "lib/prelude.h"

namespace mld {
// NOTE: shuffle edges if TLE
template <typename BipartiteGraph>
[[nodiscard]] std::vector<u32> hopcroft_karp(const BipartiteGraph &g,
                                             const std::vector<u32> &col) {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> mat(len, UNSET), lvl, lft;
    lft.reserve(len);

    std::queue<u32> q;
    for (u32 u = 0; u < len; ++u)
        if (col[u] == 1)
            q.push(u);
        else
            lft.push_back(u);

    for (u32 i = 0;;) {
        if (i == 0) {
            lvl.assign(len, len);
            std::queue<u32> bfs;
            for (u32 u : lft)
                if (mat[u] == UNSET) lvl[u] = 0, bfs.push(u);

            while (!bfs.empty()) {
                u32 u = bfs.front();
                bfs.pop();
                for (u32 v : g[u]) {
                    if (lvl[v] > lvl[u] + 1) {
                        lvl[v] = lvl[u] + 1;
                        if (u32 w = mat[v]; w != UNSET)
                            lvl[w] = lvl[v] + 1, bfs.push(w);
                    }
                }
            }
        }

        if (q.empty()) break;
        u32 v = q.front();
        q.pop();

        u32 s = UNSET, t = std::numeric_limits<u32>::max();
        for (u32 u : g[v])
            if (lvl[u] < t) t = lvl[u], s = u;

        if (s != UNSET && lvl[s] < len) {
            lvl[v] = lvl[s] + 1;
            if (u32 w = mat[s]; w != UNSET) mat[w] = UNSET, q.push(w);
            mat[v] = s;
            mat[s] = v;
            lvl[s] += 2;
        }

        if (++i == len) i = 0;
    }

    return mat;
}
} // namespace mld

#endif // MALLARD_HOPCROFT_KARP_H
