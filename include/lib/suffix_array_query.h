#ifndef MALLARD_SUFFIX_ARRAY_QUERY_H
#define MALLARD_SUFFIX_ARRAY_QUERY_H 1

#include <ranges>
#include <vector>

#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/suffix_array.h"

namespace mld {

struct suffix_array_query {
    using size_type = u32;

private:
    size_type len;

public:
    std::vector<size_type> suf, rnk;

private:
    mld::four_russian<mld::sparse_table<mld::algebra::min<size_type>>> agg;

public:
    constexpr suffix_array_query() noexcept = default;

    template <typename R>
        requires std::ranges::sized_range<R> && std::ranges::random_access_range<R> &&
                 std::integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
    constexpr explicit suffix_array_query(R &&s, u32 hi) noexcept {
        build(s, hi);
    }

    template <typename R>
        requires std::ranges::sized_range<R> && std::ranges::random_access_range<R> &&
                 std::integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
    constexpr void build(R &&s, u32 hi) noexcept {
        len = static_cast<size_type>(std::ranges::size(s));
        suf = suffix_array(s, hi);

        rnk.resize(len);
        for (u32 i = 0; i < len; ++i) rnk[suf[i]] = i;

        if (len > 1) agg.build(lcp_array(s, suf, rnk));
    }

    [[nodiscard]] constexpr size_type lcp(size_type l, size_type r) const noexcept {
        assert(l <= len && r <= len);

        if (l == r) return len - l;
        if (l == len || r == len) return 0;

        std::tie(l, r) = std::minmax(rnk[l], rnk[r]);
        return agg.fold(l, r).val();
    }

    [[nodiscard]] constexpr auto compare_suffix(size_type a,
                                                size_type b) const noexcept {
        assert(a <= len && b <= len);
        return rnk[a] <=> rnk[b];
    }

    [[nodiscard]] constexpr bool equate_substr(size_type l1, size_type l2,
                                               size_type k) const noexcept {
        assert(l1 < len && l1 + k <= len);
        assert(l2 < len && l2 + k <= len);

        return lcp(l1, l2) >= k;
    }

    [[nodiscard]] constexpr auto compare_substr(size_type l1, size_type r1,
                                                size_type l2,
                                                size_type r2) const noexcept {
        assert(l1 < r1 && r1 <= len);
        assert(l2 < r2 && r2 <= len);

        size_type len1 = r1 - l1, len2 = r2 - l2;
        return lcp(l1, l2) >= std::min(len1, len2) ? len1 <=> len2
                                                   : compare_suffix(l1, l2);
    }

    template <typename R>
        requires std::ranges::sized_range<R> && std::ranges::random_access_range<R> &&
                 std::integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
    [[nodiscard]] constexpr std::tuple<size_type, size_type, size_type, size_type>
    search(R &&s, R &&t) const noexcept {
        size_type p = len, q = 0;
        auto cmp = [&](size_type i, size_type) -> bool {
            if (size_type r = p == len ? 0 : lcp(p, i); r != q)
                return (r < q) ^ (compare_suffix(p, i) < 0);
            auto [it, ti] =
                std::mismatch(i + q + std::ranges::begin(s), std::ranges::end(s),
                              q + std::ranges::begin(t), std::ranges::end(t));
            p = i, q = static_cast<size_type>(ti - std::ranges::begin(t));
            return std::lexicographical_compare(it, std::ranges::end(s), ti,
                                                std::ranges::end(t));
        };

        size_type l = static_cast<size_type>(std::ranges::lower_bound(suf, 0, cmp) -
                                             suf.begin()),
                  r = l;
        if (q == std::ranges::size(t)) {
            auto it = std::lower_bound(l + suf.begin(), suf.end(), 0,
                                       [&](size_type i, size_type) -> bool {
                                           return lcp(p, i) >= std::ranges::size(t);
                                       });
            r = static_cast<size_type>(it - suf.begin());
        }

        return {l, r, p, p + q};
    }
};
} // namespace mld

#endif // MALLARD_SUFFIX_ARRAY_QUERY_H
