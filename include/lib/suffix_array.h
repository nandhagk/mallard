#ifndef MALLARD_SUFFIX_ARRAY_H
#define MALLARD_SUFFIX_ARRAY_H 1

#include <algorithm>
#include <cassert>
#include <concepts>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
[[nodiscard]] constexpr std::vector<u32> suffix_array_naive(R &&s) noexcept {
    u32 len = static_cast<u32>(std::ranges::size(s));

    std::vector<u32> sa(len);
    std::iota(sa.begin(), sa.end(), 0);

    std::ranges::sort(sa, [&](u32 l, u32 r) {
        if (l == r) return false;
        for (; l < len && r < len; ++l, ++r)
            if (s[l] != s[r]) return s[l] < s[r];
        return l == len;
    });

    return sa;
}

template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::input_range<R> &&
             std::integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
[[nodiscard]] constexpr std::vector<u32> suffix_array_doubling(R &&s) noexcept {
    u32 len = static_cast<u32>(std::ranges::size(s));

    std::vector<u32> sa(len);
    std::iota(sa.begin(), sa.end(), 0);

    std::vector<u32> rnk(std::ranges::begin(s), std::ranges::end(s)), tmp(len);
    for (u32 k = 1; k < len; k *= 2) {
        auto cmp = [&](u32 x, u32 y) {
            if (rnk[x] != rnk[y]) return rnk[x] < rnk[y];
            if (x + k < len && y + k < len) return rnk[x + k] < rnk[y + k];
            return y + k < len;
        };
        std::sort(sa.begin(), sa.end(), cmp);

        tmp[sa[0]] = 0;
        for (u32 i = 1; i < len; ++i)
            tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);

        std::swap(tmp, rnk);
    }

    return sa;
}

// SA-IS, linear-time suffix array construction
// Reference:
// G. Nong, S. Zhang, and W. H. Chan,
// Two Efficient Algorithms for Linear Time Suffix Array Construction
template <u32 THRESHOLD_NAIVE = 10, u32 THRESHOLD_DOUBLING = 40, typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R> &&
             std::integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
[[nodiscard]] constexpr std::vector<u32> suffix_array(R &&s, u32 hi) noexcept {
    constexpr u32 UNSET = -1;
    u32 len = static_cast<u32>(std::ranges::size(s));

    if (len == 0) return {};
    if (len == 1) return {0};
    if (len == 2) {
        if (s[0] < s[1])
            return {0, 1};
        else
            return {1, 0};
    }

    if (len < THRESHOLD_NAIVE) return suffix_array_naive(s);
    if (len < THRESHOLD_DOUBLING) return suffix_array_doubling(s);

    std::vector<u32> sa(len);
    std::vector<bool> ls(len);
    for (u32 i = len - 1; i--;)
        ls[i] = (s[i] == s[i + 1]) ? ls[i + 1] : (s[i] < s[i + 1]);

    std::vector<u32> sum_l(hi + 1), sum_s(hi + 1);
    for (u32 i = 0; i < len; i++)
        if (!ls[i])
            ++sum_s[s[i]];
        else
            ++sum_l[s[i] + 1];

    for (u32 i = 0; i <= hi; ++i) {
        sum_s[i] += sum_l[i];
        if (i < hi) sum_l[i + 1] += sum_s[i];
    }

    auto induce = [&](const std::vector<u32> &lms) {
        std::ranges::fill(sa, UNSET);

        std::vector<u32> buf(hi + 1);
        std::ranges::copy(sum_s, buf.begin());

        for (u32 d : lms)
            if (d != len) sa[buf[s[d]]++] = d;

        std::ranges::copy(sum_l, buf.begin());
        sa[buf[s[len - 1]]++] = len - 1;
        for (u32 v : sa)
            if (v != UNSET && v >= 1 && !ls[v - 1]) sa[buf[s[v - 1]]++] = v - 1;

        std::ranges::copy(sum_l, buf.begin());
        for (u32 v : sa | std::views::reverse)
            if (v != UNSET && v >= 1 && ls[v - 1]) sa[--buf[s[v - 1] + 1]] = v - 1;
    };

    std::vector<u32> lms_map(len + 1, UNSET);
    u32 n = 0;
    for (u32 i = 1; i < len; ++i)
        if (!ls[i - 1] && ls[i]) lms_map[i] = n++;

    std::vector<u32> lms;
    lms.reserve(n);
    for (u32 i = 1; i < len; ++i)
        if (!ls[i - 1] && ls[i]) lms.push_back(i);

    induce(lms);

    if (n) {
        std::vector<u32> sorted_lms;
        sorted_lms.reserve(n);
        for (u32 v : sa)
            if (lms_map[v] != UNSET) sorted_lms.push_back(v);

        std::vector<u32> rec_s(n);
        u32 rec_upper = 0;
        rec_s[lms_map[sorted_lms[0]]] = 0;
        for (u32 i = 1; i < n; ++i) {
            u32 l = sorted_lms[i - 1], r = sorted_lms[i];
            u32 end_l = (lms_map[l] + 1 < n) ? lms[lms_map[l] + 1] : len;
            u32 end_r = (lms_map[r] + 1 < n) ? lms[lms_map[r] + 1] : len;
            bool same = true;
            if (end_l - l != end_r - r) {
                same = false;
            } else {
                for (; l < end_l && s[l] == s[r]; ++l, ++r);
                if (l == len || s[l] != s[r]) same = false;
            }
            if (!same) ++rec_upper;
            rec_s[lms_map[sorted_lms[i]]] = rec_upper;
        }

        auto rec_sa =
            suffix_array<THRESHOLD_NAIVE, THRESHOLD_DOUBLING>(rec_s, rec_upper);

        for (u32 i = 0; i < n; ++i) sorted_lms[i] = lms[rec_sa[i]];
        induce(sorted_lms);
    }

    return sa;
}
} // namespace mld

#endif // MALLARD_SUFFIX_ARRAY_H
