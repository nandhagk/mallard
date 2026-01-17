#ifndef MALLARD_BLOCK_CUT_TREE_H
#define MALLARD_BLOCK_CUT_TREE_H 1

#include <vector>

#include "lib/dynamic_graph.h"
#include "lib/prelude.h"

namespace mld {

template <typename UndirectedGraph>
[[nodiscard]] constexpr std::tuple<u32, std::vector<u32>, dynamic_graph<u32>>
block_cut_tree(const UndirectedGraph &g, const std::vector<bool> &cut,
               const std::vector<std::vector<u32>> &ccs) noexcept {
    u32 len = static_cast<u32>(g.size());
    std::vector<u32> ids(len);

    u32 grp = 0;
    for (u32 u = 0; u < len; ++u)
        if (cut[u]) ids[u] = grp++;

    dynamic_graph<u32> bct(grp + static_cast<u32>(ccs.size()));
    bct.reserve(2 * bct.size());

    for (auto &&cc : ccs) {
        for (u32 u : cc) {
            if (!cut[u]) {
                ids[u] = grp;
                continue;
            }

            bct.add_edge(ids[u], grp);
            bct.add_edge(grp, ids[u]);
        }

        ++grp;
    }

    return {grp, std::move(ids), std::move(bct)};
}
} // namespace mld

#endif // MALLARD_BLOCK_CUT_TREE_H
