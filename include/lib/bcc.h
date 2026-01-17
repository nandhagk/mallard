#ifndef MALLARD_BCC_H
#define MALLARD_BCC_H 1

#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename UndirectedGraph>
[[nodiscard]] constexpr std::pair<std::vector<bool>, std::vector<std::vector<u32>>>
bcc(const UndirectedGraph &g) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(g.size());

    std::vector<u32> sen, tin(len, UNSET), low(len);
    sen.reserve(len);

    std::vector<std::vector<u32>> ccs;
    ccs.reserve(len);

    std::vector<bool> cut(len);

    u32 tim = 0;
    auto dfs = [&](auto &&self, u32 u, u32 t) -> void {
        sen.push_back(u);
        tin[u] = low[u] = tim++;

        u32 chi = 0;
        for (u32 v : g[u]) {
            if (v == t) continue;

            if (tin[v] != UNSET) {
                low[u] = std::min(low[u], tin[v]);
                continue;
            }

            ++chi;
            u32 s = static_cast<u32>(sen.size());

            self(self, v, u);
            low[u] = std::min(low[u], low[v]);

            if ((t == UNSET && chi > 1) || (t != UNSET && low[v] >= tin[u])) {
                cut[u] = true;
                auto &cc = ccs.back();

                cc.push_back(u);
                for (; static_cast<u32>(sen.size()) > s; sen.pop_back())
                    cc.push_back(sen.back());

                ccs.emplace_back();
            }
        }
    };

    for (u32 u = 0; u < len; ++u) {
        if (tin[u] != UNSET) continue;

        ccs.emplace_back();
        dfs(dfs, u, UNSET);

        auto &cc = ccs.back();
        for (u32 v : sen) cc.push_back(v);

        sen.clear();
    }

    return {std::move(cut), std::move(ccs)};
}
} // namespace mld

#endif // MALLARD_BCC_H
