#ifndef MALLARD_STATIC_MODE_H
#define MALLARD_STATIC_MODE_H 1

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

namespace mld {
struct static_mode {
    using word_type = u32;
    using size_type = u32;

private:
    size_type len, blk;
    std::vector<word_type> val;
    std::vector<size_type> rnk;
    static_csr<size_type> pos;
    static_csr<std::pair<size_type, word_type>> lrg;

public:
    constexpr static_mode() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<word_type, R>
    explicit constexpr static_mode(R &&r, size_type sz) noexcept {
        build(std::forward<R>(r), sz);
    }

    template <typename R>
        requires internal::sized_input_range_of<word_type, R>
    constexpr void build(R &&r, size_type sz) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        blk = static_cast<size_type>(std::sqrt(len));

        val.clear(), val.reserve(len);
        for (auto &&x : r) val.emplace_back(x);

        rnk.resize(len);

        size_type cap = (len + blk - 1) / blk + 1;
        lrg.build(std::views::iota(size_type{0}, cap) |
                  std::views::transform([&](auto) { return cap; }));

        std::vector<size_type> cnt(sz);
        rnk.clear(), rnk.reserve(len);
        for (auto &&x : val) rnk.push_back(cnt[x]++);

        pos.build(cnt);

        size_type z = 0;
        for (auto &&x : val) pos[x][rnk[z]] = z, ++z;

        for (size_type i = 0; i * blk < len; ++i) {
            std::ranges::fill(cnt, 0);
            size_type p = i * blk, q = std::min(len, p + blk);
            std::pair<size_type, word_type> m;
            for (size_type j = p, k = i + 1; j < len; ++j) {
                ++cnt[val[j]];
                m = std::max(m, {cnt[val[j]], val[j]});
                if (j + 1 == q) {
                    lrg[i][k++] = m;
                    q = std::min(len, q + blk);
                }
            }
        }
    }

    [[nodiscard]] constexpr std::pair<word_type, size_type>
    mode(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);

        size_type L = (l + blk - 1) / blk, R = r / blk;
        std::pair<size_type, word_type> m;

        auto lft = [&](size_type p, size_type q) {
            for (size_type i = p; i < q; ++i) {
                word_type x = val[i];
                size_type k = rnk[i];
                if (k + m.first >= static_cast<size_type>(pos[x].size()) ||
                    pos[x][k + m.first] >= r) {
                    continue;
                }

                size_type j = k + m.first;
                for (; j < static_cast<size_type>(pos[x].size()) && pos[x][j] < r; ++j);
                m = std::max(m, {j - k, x});
            }
        };

        auto rgt = [&](size_type p, size_type q) {
            for (size_type i = p; i >= q; --i) {
                word_type x = val[i];
                size_type k = rnk[i];
                if (k < m.first || pos[x][k - m.first] < l) continue;
                size_type j = k - m.first + 1;
                for (; j-- && pos[x][j] >= l;);
                m = std::max(m, {k - j, x});
            }
        };

        if (L >= R) {
            lft(l, r);
        } else {
            m = lrg[L][R];
            lft(l, L * blk);
            rgt(r - 1, R * blk);
        }

        return {m.second, m.first};
    }
};
} // namespace mld

#endif // MALLARD_STATIC_MODE_H
