#ifndef MALLARD_CHROMATIC_PARTITION_H
#define MALLARD_CHROMATIC_PARTITION_H 1

#include <limits>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename UndirectedGraph>
[[nodiscard]] constexpr std::pair<u32, std::vector<u32>>
chromatic_partition(const UndirectedGraph &g) noexcept {
    u32 len = static_cast<u32>(g.size());
    assert(len < std::numeric_limits<u64>::digits);

    u32 grp = len + 1;
    std::vector<u32> col;

    auto dfs = [&](auto &&self, u32 c, u32 cnt) -> void {
        static std::vector<u32> cur(len);

        if (c >= grp) return;
        if (!cnt) return grp = c, col = cur, void();

        constexpr u32 UNSET = -1;
        u32 u = UNSET;

        u64 q = 0;
        for (u32 i = 0, d = 0; i < len; ++i) {
            if (!cur[i]) {
                u64 s = 0;
                for (u32 j : g[i])
                    if (cur[j]) s |= u64(1) << cur[j];
                u32 t = std::popcount(s);
                if (t >= d) d = t, u = i, q = s;
            }
        }

        for (u32 i = 1; i <= c + 1; ++i)
            if (!((q >> i) & 1)) cur[u] = i, self(self, std::max(c, i), cnt - 1);

        cur[u] = 0;
    };

    dfs(dfs, 0, len);
    std::ranges::transform(col, col.begin(), [](u32 c) { return --c; });
    return {grp, std::move(col)};
}
} // namespace mld

#endif // MALLARD_CHROMATIC_PARTITION_H
