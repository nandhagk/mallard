#ifndef MALLARD_HAVEL_HAKIMI_H
#define MALLARD_HAVEL_HAKIMI_H 1

#include <vector>

#include "lib/prelude.h"

namespace mld {
[[nodiscard]] constexpr std::vector<std::pair<u32, u32>> havel_hakimi(std::vector<u32> deg) noexcept {
    u32 n = static_cast<u32>(deg.size());

    std::vector<std::vector<u32>> d(n);
    for (u32 u = 0; u < n; ++u) d[deg[u]].push_back(u);

    std::vector<std::pair<u32, u32>> e;
    e.reserve(std::reduce(deg.begin(), deg.end()));

    i32 mx = n - 1;
    for (u32 i = 0; i < n; ++i) {
        while (mx >= 0 && d[mx].empty()) --mx;

        u32 u = d[mx].back();
        d[mx].pop_back();

        std::vector<u32> nbd;

        u32 k = mx;
        while (static_cast<u32>(nbd.size()) < deg[u]) {
            assert(k != 0);
            if (d[k].empty()) {
                --k;
                continue;
            }

            u32 v = d[k].back();
            d[k].pop_back();

            nbd.push_back(v);
        }

        for (u32 v : nbd) {
            e.emplace_back(u, v);

            --deg[v];
            d[deg[v]].push_back(v);
        }

        deg[u] = 0;
    }

    return e;
}
} // namespace mld

#endif // MALLARD_HAVEL_HAKIMI_H
