#ifndef MALLARD_CCC_H
#define MALLARD_CCC_H 1

#include <numeric>
#include <queue>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename UndirectedGraph>
[[nodiscard]] std::pair<u32, std::vector<u32>> ccc(const UndirectedGraph &g) noexcept {
    u32 len = static_cast<u32>(g.size());
    constexpr u32 UNSET = -1;

    std::vector<u32> ids(len, UNSET), rem(len), flg(len);
    std::iota(rem.begin(), rem.end(), 0);

    u32 grp = 0;
    for (std::queue<u32> q; !rem.empty(); ++grp) {
        u32 r = rem.back();
        rem.pop_back();

        ids[r] = grp;
        q.push(r);
        while (!q.empty()) {
            u32 u = q.front();
            q.pop();

            for (u32 v : g[u]) flg[v] = 1;

            for (u32 i = 0; i < rem.size(); ++i) {
                u32 v = rem[i];
                if (flg[v]) continue;

                if (ids[v] == UNSET) {
                    ids[v] = grp;
                    q.push(v);
                }

                std::swap(rem[i--], rem.back());
                rem.pop_back();
            }

            for (u32 v : g[u]) flg[v] = 0;
        }
    }

    return {grp, std::move(ids)};
}
} // namespace mld

#endif // MALLARD_CCC_H
