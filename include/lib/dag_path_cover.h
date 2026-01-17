#ifndef MALLARD_DAG_PATH_COVER_H
#define MALLARD_DAG_PATH_COVER_H 1

#include <algorithm>
#include <vector>

#include "lib/hopcroft_karp.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

namespace mld {

template <typename DAG>
[[nodiscard]] constexpr static_csr<u32> dag_path_cover(const DAG &g,
                                                       u32 m = 0) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<std::pair<u32, u32>> e;
    if (m != 0) e.reserve(2 * m);

    for (u32 u = 0; u < len; ++u) {
        for (u32 v : g[u]) {
            e.emplace_back(u, v + len);
            e.emplace_back(v + len, u);
        }
    }

    // std::ranges::shuffle(e, MT);
    static_csr h(2 * len, e);

    std::vector<u32> col(2 * len, 0);
    std::fill(col.begin() + len, col.end(), 1);

    auto mat = hopcroft_karp(h, col);

    std::vector<u32> ord;
    ord.reserve(len);

    for (u32 u = len; u < 2 * len; ++u)
        if (mat[u] == UNSET) ord.push_back(u - len);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(len);

    u32 grp = 0;
    for (u32 u : ord) {
        for (;; u = mat[u] - len) {
            f.emplace_back(grp, u);
            if (mat[u] == UNSET || mat[u] < len) break;
        }
        ++grp;
    }

    // static_csr inverts order
    std::ranges::reverse(f);

    static_csr pth(grp, f);
    return pth;
}
} // namespace mld

#endif // MALLARD_DAG_PATH_COVER_H 1
