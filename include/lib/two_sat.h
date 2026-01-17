#ifndef MALLARD_TWO_SAT_H
#define MALLARD_TWO_SAT_H 1

#include <optional>

#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/static_csr.h"

namespace mld {
struct two_sat {
    using size_type = u32;

private:
    size_type len;
    std::vector<std::pair<u32, u32>> e;

public:
    constexpr two_sat() noexcept = default;
    explicit constexpr two_sat(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        e.clear();
    }

    constexpr void reserve(size_type n) noexcept {
        e.reserve(2 * n);
    }

    constexpr void add_clause(size_type i, bool p, size_type j, bool q) noexcept {
        assert(i < len && j < len);

        e.emplace_back(2 * i + (p ? 0 : 1), 2 * j + (q ? 1 : 0));
        e.emplace_back(2 * j + (q ? 0 : 1), 2 * i + (p ? 1 : 0));
    }

    constexpr std::optional<std::vector<bool>> satisfiable() const noexcept {
        std::vector<bool> ans(len, false);

        mld::static_csr g(2 * len, e);

        auto [_, ids] = scc(g);
        for (u32 i = 0; i < len; ++i) {
            if (ids[2 * i] == ids[2 * i + 1]) return std::nullopt;
            ans[i] = ids[2 * i] < ids[2 * i + 1];
        }

        return ans;
    }
};
} // namespace mld

#endif // MALLARD_TWO_SAT_H
