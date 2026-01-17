#ifndef MALLARD_FOUR_RUSSIAN_H
#define MALLARD_FOUR_RUSSIAN_H 1

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <algebra::internal::foldable Aggregate, u32 log = 4>
struct four_russian {
    using aggregate = Aggregate;
    using operand = aggregate::operand;

    using size_type = u32;
    using container_type = std::vector<operand>;

private:
    static const constexpr size_type mask = (1 << log) - 1;

    size_type len;
    container_type buf, pre, suf;
    aggregate agg;

public:
    constexpr four_russian() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R> &&
                 algebra::internal::buildable<aggregate, R>
    explicit constexpr four_russian(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R> &&
                 algebra::internal::buildable<aggregate, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        assert(len > 0);

        buf.resize(len);
        std::ranges::copy(r, buf.begin());

        pre = suf = buf;

        for (size_type i = 1; i < len; ++i)
            if (i & mask) pre[i] = pre[i - 1] + buf[i];

        for (size_type i = len - 1; i != 0; --i)
            if (i & mask) suf[i - 1] = buf[i - 1] + suf[i];

        agg.build(std::views::iota(size_type{0}, len >> log) |
                  std::views::transform([this](auto k) { return suf[k << log]; }));
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);

        --r;
        if (size_type a = (l >> log) + 1, b = r >> log; a < b)
            return suf[l] + agg.fold(a, b) + pre[r];
        else if (a == b)
            return suf[l] + pre[r];

        if constexpr (algebra::internal::commutative<operand>) {
            return std::reduce(buf.begin() + (l + 1), buf.begin() + (r + 1), buf[l]);
        } else {
            return std::accumulate(buf.begin() + (l + 1), buf.begin() + (r + 1),
                                   buf[l]);
        }
    }
};
} // namespace mld

#endif // MALLARD_FOUR_RUSSIAN_H
