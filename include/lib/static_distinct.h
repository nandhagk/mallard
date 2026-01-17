#ifndef MALLARD_STATIC_DISTINCT_H
#define MALLARD_STATIC_DISTINCT_H 1

#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/wavelet_matrix.h"

namespace mld {
struct static_distinct {
    using word_type = typename wavelet_matrix::word_type;
    using size_type = typename wavelet_matrix::size_type;

    constexpr static_distinct() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<word_type, R>
    constexpr static_distinct(R &&r, word_type sz) noexcept {
        build(std::forward<R>(r), sz);
    }

    template <typename R>
        requires internal::sized_input_range_of<word_type, R>
    constexpr void build(R &&r, word_type sz) noexcept {
        size_type len = static_cast<size_type>(std::ranges::size(r));
        std::vector<size_type> prv(sz, len);
        std::vector<word_type> nxt(len + 1, len);

        size_type i = 0;
        for (auto &&a : r) prv[a] = nxt[prv[a]] = i++;

        wm.build(std::span(nxt.data(), nxt.data() + len), len + 1);
    }

    [[nodiscard]] constexpr size_type count(size_type l, size_type r) const noexcept {
        return r - l - wm.rank(l, r, r);
    }

private:
    mld::wavelet_matrix wm;
};
} // namespace mld

#endif // MALLARD_STATIC_DISTINCT_H
