#ifndef MALLARD_WAVELET_MATRIX_H
#define MALLARD_WAVELET_MATRIX_H 1

#include <algorithm>
#include <bit>
#include <cassert>
#include <numeric>
#include <utility>
#include <vector>

#include "lib/bit_vector.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

struct wavelet_matrix {
    using word_type = u32;
    using size_type = u32;
    using container_type = bit_vector<u64>;

private:
    word_type cap;
    size_type len, log;
    std::vector<container_type> buf;

public:
    constexpr wavelet_matrix() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<word_type, R>
    constexpr explicit wavelet_matrix(R &&r, word_type sz) noexcept {
        build(std::forward<R>(r), sz);
    }

    template <typename R>
        requires internal::sized_input_range_of<word_type, R>
    constexpr void build(R &&r, word_type sz) noexcept {
        cap = sz;
        len = static_cast<size_type>(std::ranges::size(r));
        log = std::max<size_type>(1, std::countr_zero(std::bit_ceil(cap)));

        buf.assign(log, container_type(len));
        std::vector<word_type> cur(std::ranges::begin(r), std::ranges::end(r)),
            nxt(len);

        for (size_type h = log; h--;) {
            for (size_type i = 0; i < len; ++i)
                if (cur[i] >> h & 1) buf[h].set(i);
            buf[h].build();

            std::array it{nxt.begin(), nxt.begin() + buf[h].zeros};
            for (size_type i = 0; i < len; ++i) *it[buf[h][i]]++ = cur[i];
            std::swap(cur, nxt);
        }
    }

    [[nodiscard]] constexpr word_type kth(size_type l, size_type r,
                                          size_type k) const noexcept {
        assert(l < r && r <= len && k < r - l);

        word_type v = 0;
        for (size_type h = log; h--;) {
            if (size_type a = buf[h].rank0(l), b = buf[h].rank0(r); k < b - a) {
                l = a, r = b;
            } else {
                k -= b - a;
                v |= word_type{1} << h;
                l += buf[h].zeros - a, r += buf[h].zeros - b;
            }
        }

        return v;
    }

    [[nodiscard]] constexpr word_type
    kth(std::vector<std::pair<size_type, size_type>> segments,
        size_type k) const noexcept {
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
                    l += buf[h].zeros - buf[h].rank0(l);
                    r += buf[h].zeros - buf[h].rank0(r);
                }
            }
        }

        return v;
    }

    [[nodiscard]] constexpr size_type rank(size_type l, size_type r,
                                           word_type v) const noexcept {
        assert(l < r && r <= len);
        if (v >= cap) return r - l;

        size_type z = 0;
        for (size_type h = log; h--;) {
            if (size_type a = buf[h].rank0(l), b = buf[h].rank0(r); ~v >> h & 1) {
                l = a, r = b;
            } else {
                z += b - a;
                l += buf[h].zeros - a, r += buf[h].zeros - b;
            }
        }

        return z;
    }
};

} // namespace mld

#endif // MALLARD_WAVELET_MATRIX_H
