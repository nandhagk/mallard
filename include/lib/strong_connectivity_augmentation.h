#ifndef MALLARD_STRONG_CONNECTIVITY_AUGMENTATION_H
#define MALLARD_STRONG_CONNECTIVITY_AUGMENTATION_H 1

#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename DAG>
[[nodiscard]] constexpr std::vector<std::pair<u32, u32>>
strong_connectivity_augmentation(const DAG &g) noexcept {
    u32 len = static_cast<u32>(g.size());
    if (len == 1) return {};

    std::vector<bool> zero_in(len, true);
    for (u32 u = 0; u < len; ++u)
        for (u32 v : g[u]) zero_in[v] = false;

    std::vector<bool> seen(len, false);

    constexpr u32 UNSET = -1;
    auto dfs = [&](auto &&self, u32 u) -> u32 {
        bool flag = false;
        for (u32 v : g[u]) {
            flag = true;
            if (seen[v]) continue;

            seen[v] = true;
            if (u32 zero_out = self(self, v); zero_out != UNSET) return zero_out;
        }

        return flag ? UNSET : u;
    };

    std::vector<std::pair<u32, u32>> e;
    std::vector<u32> in_unused;

    for (u32 u = 0; u < len; ++u) {
        if (!zero_in[u]) continue;

        seen[u] = true;
        if (u32 zero_out = dfs(dfs, u); zero_out != UNSET)
            e.emplace_back(zero_out, u);
        else
            in_unused.push_back(u);
    }

    for (u32 i = 1; i < static_cast<u32>(e.size()); ++i)
        std::swap(e[i].first, e[i - 1].first);

    for (u32 u = 0; u < len; ++u) {
        if (!g[u].empty() || seen[u]) continue;

        if (!in_unused.empty()) {
            e.emplace_back(u, in_unused.back());
            in_unused.pop_back();
        } else {
            e.emplace_back(u, 0);
        }
    }

    for (u32 u : in_unused) e.emplace_back(len - 1, u);
    return e;
}
} // namespace mld

#endif // MALLARD_STRONG_CONNECTIVITY_AUGMENTATION_H
