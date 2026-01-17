#ifndef MALLARD_OFFLINE_STATIC_DISTINCT_H
#define MALLARD_OFFLINE_STATIC_DISTINCT_H 1

#include <vector>

#include "lib/dynamic_csr.h"
#include "lib/fenwick_set.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::offline {
struct static_distinct {
    using size_type = u32;

private:
    static constexpr size_type UNSET = -1;

    size_type len, cap;
    std::vector<size_type> nxt;
    fenwick_set<> fws;
    dynamic_csr<std::pair<size_type, size_type>> que;

public:
    constexpr static_distinct() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    constexpr static_distinct(R &&r, size_type sz) noexcept {
        build(std::forward<R>(r), sz);
    }

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    constexpr void build(R &&r, size_type sz) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));

        nxt.assign(len, len);
        fws.build(len + 1);

        std::vector<size_type> prv(sz, UNSET);

        size_type i = 0;
        for (auto &&a : r) {
            if (prv[a] == UNSET)
                fws.insert(i);
            else
                nxt[prv[a]] = i;
            prv[a] = i++;
        }

        que.build(len + 1);
    }

    constexpr void reserve(size_type q) noexcept {
        que.reserve(q);
    }

    constexpr void query(size_type l, size_type r) noexcept {
        if (l < r) que[l].emplace_back(r, cap);
        ++cap;
    }

    [[nodiscard]] constexpr std::vector<size_type> solve() noexcept {
        std::vector<size_type> out(cap);

        for (u32 l = 0; l < len; ++l) {
            for (auto &&[r, j] : que[l]) out[j] = fws.rank(l, r);
            fws.insert(nxt[l]);
        }

        return out;
    }
};
} // namespace mld::offline

#endif // MALLARD_OFFLINE_STATIC_DISTINCT_H
