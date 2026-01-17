#ifndef MALLARD_OFFLINE_STATIC_REACHABILITY_H
#define MALLARD_OFFLINE_STATIC_REACHABILITY_H 1

#include <algorithm>

#include "lib/dynamic_csr.h"
#include "lib/scc.h"

namespace mld::offline {

template <typename WordT = u64>
struct static_reachability {
    using word_type = WordT;
    using size_type = u32;

private:
    static constexpr size_type word_size = sizeof(word_type) * 8;

    size_type len, cap;
    std::vector<size_type> ids;
    std::vector<std::pair<size_type, size_type>> edg;
    dynamic_csr<std::pair<size_type, size_type>> que;

public:
    template <typename DirectedGraph>
    constexpr static_reachability(const DirectedGraph &g) noexcept {
        std::tie(len, ids) = scc(g);

        for (size_type u = 0; u < g.size(); ++u) {
            for (size_type v : g[u])
                if (ids[u] != ids[v]) edg.emplace_back(ids[u], ids[v]);
        }

        std::ranges::sort(edg, std::greater{});
        edg.erase(std::unique(edg.begin(), edg.end()), edg.end());

        que.build(len);
    }

    constexpr void reserve(size_type n) noexcept {
        que.reserve(n);
    }

    constexpr void query(size_type u, size_type v) noexcept {
        if (ids[u] <= ids[v]) que[ids[v]].emplace_back(ids[u], cap);
        ++cap;
    }

    [[nodiscard]] constexpr std::vector<bool> solve() const noexcept {
        std::vector<bool> ans(cap);

        std::vector<word_type> dp(len);
        for (u32 l = 0; l < len; l += word_size) {
            u32 r = std::min(len, l + word_size);

            std::ranges::fill(dp, 0);
            for (u32 u = l; u < r; ++u) dp[u] = word_type{1} << (u - l);
            for (auto &&[u, v] : edg) dp[u] |= dp[v];
            for (u32 v = l; v < r; ++v)
                for (auto &&[u, i] : que[v]) ans[i] = (dp[u] >> (v - l)) & 1;
        }

        return ans;
    }
};
} // namespace mld::offline

#endif // MALLARD_OFFLINE_STATIC_REACHABILITY_H
