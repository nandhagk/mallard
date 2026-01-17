#ifndef MALLARD_WAVELET_MATRIX_AGGREGATE_H
#define MALLARD_WAVELET_MATRIX_AGGREGATE_H 1

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/bit_vector.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

template <algebra::internal::foldable Aggregate>
struct wavelet_matrix_aggregate {
    using aggregate = Aggregate;

    using operand = aggregate::operand;
    static_assert(algebra::internal::commutative<operand>);

    using word_type = u32;
    using size_type = u32;
    using container_type = bit_vector<u64>;

private:
    word_type cap;
    size_type len, log;
    std::vector<container_type> buf;
    std::vector<aggregate> agg;

public:
    constexpr wavelet_matrix_aggregate() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<std::pair<word_type, operand>, R> &&
                 algebra::internal::buildable<aggregate,
                                              std::ranges::elements_view<R, 1>>
    constexpr explicit wavelet_matrix_aggregate(R &&r, word_type sz) noexcept {
        build(std::forward<R>(r), sz);
    }

    template <typename R>
        requires internal::sized_input_range_of<std::pair<word_type, operand>, R> &&
                 algebra::internal::buildable<aggregate,
                                              std::ranges::elements_view<R, 1>>
    constexpr void build(R &&r, word_type sz) noexcept {
        cap = sz;
        len = static_cast<size_type>(std::ranges::size(r));
        log = std::max<size_type>(1, std::countr_zero(std::bit_ceil(cap)));

        buf.assign(log, container_type(len));
        agg.resize(log);

        std::vector<std::pair<word_type, operand>> cur(std::ranges::begin(r),
                                                       std::ranges::end(r)),
            nxt(len);

        for (size_type h = log; h--;) {
            for (size_type i = 0; i < len; ++i)
                if (cur[i].first >> h & 1) buf[h].set(i);

            buf[h].build();

            std::array it{nxt.begin(), nxt.begin() + buf[h].zeros};
            for (size_type i = 0; i < len; ++i) *it[buf[h][i]]++ = cur[i];
            std::swap(cur, nxt);

            agg[h].build(cur | std::views::elements<1>);
        }
    }

    [[nodiscard]] constexpr std::pair<word_type, operand>
    kth(size_type l, size_type r, size_type k) const noexcept {
        assert(l < r && r <= len && k < r - l);
        operand x;

        word_type v = 0;
        for (size_type h = log; h--;) {
            if (size_type a = buf[h].rank0(l), b = buf[h].rank0(r); k < b - a) {
                l = a, r = b;
            } else {
                k -= b - a;
                v |= word_type{1} << h;
                l += buf[h].zeros - a, r += buf[h].zeros - b;
                if (a < b) x = x + agg[h].fold(a, b);
            }
        }

        if (k != 0) x = x + agg[0].fold(l, l + k);
        return {v, x};
    }

    [[nodiscard]] constexpr std::pair<word_type, operand>
    kth(std::vector<std::pair<size_type, size_type>> segments,
        size_type k) const noexcept {
        operand x;

        word_type v = 0;
        for (size_type h = log; h--;) {
            size_type c =
                std::transform_reduce(segments.begin(), segments.end(), size_type{0},
                                      std::plus<>{}, [&](auto &&seg) {
                                          auto &&[l, r] = seg;
                                          return buf[h].rank0(r) - buf[h].rank0(l);
                                      });
            if (k < c) {
                for (auto &&[l, r] : segments) l = buf[h].rank0(l), r = buf[h].rank0(r);
            } else {
                k -= c;
                v |= word_type{1} << h;
                for (auto &&[l, r] : segments) {
                    size_type a = buf[h].rank0(l), b = buf[h].rank0(r);
                    l += buf[h].zeros - a, r += buf[h].zeros - b;
                    if (a < b) x = x + agg[h].fold(a, b);
                }
            }
        }

        for (auto &&[l, r] : segments) {
            if (k == 0) break;

            size_type t = std::min(r - l, k);
            x = x + agg[0].fold(l, l + t), k -= t;
        }

        return {v, x};
    }

    // TODO: Fix when v >= cap
    [[nodiscard]] constexpr std::pair<size_type, operand>
    rank(size_type l, size_type r, word_type v) const noexcept {
        assert(l < r && r <= len);
        operand x;

        size_type z = 0;
        for (size_type h = log; h--;) {
            if (size_type a = buf[h].rank0(l), b = buf[h].rank0(r); ~v >> h & 1) {
                l = a, r = b;
            } else {
                z += b - a;
                l += buf[h].zeros - a, r += buf[h].zeros - b;
                if (a < b) x = x + agg[h].fold(a, b);
            }
        }

        return {z, x};
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(k < len);

        for (size_type h = log, l = k, r = k + 1; h--;) {
            if (size_type a = buf[h].rank0(l), b = buf[h].rank0(r); a < b)
                l = a, r = b;
            else
                l += buf[h].zeros - a, r += buf[h].zeros - b;

            agg[h].set(l, x);
        }
    }

    constexpr void add(size_type k, const operand &x) noexcept {
        assert(k < len);

        for (size_type h = log, l = k, r = k + 1; h--;) {
            if (size_type a = buf[h].rank0(l), b = buf[h].rank0(r); a < b)
                l = a, r = b;
            else
                l += buf[h].zeros - a, r += buf[h].zeros - b;

            agg[h].add(l, x);
        }
    }
};

} // namespace mld

#endif // MALLARD_WAVELET_MATRIX_AGGREGATE_H
