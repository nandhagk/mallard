#ifndef MALLARD_DISJOINT_SPARSE_TABLE_H
#define MALLARD_DISJOINT_SPARSE_TABLE_H 1

#include <algorithm>
#include <cassert>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

namespace mld {
template <algebra::internal::semigroup Semigroup>
struct disjoint_sparse_table {
    using operand = Semigroup;

    using size_type = u32;
    using container_type = static_csr<operand>;

private:
    size_type len, log;
    container_type buf;

public:
    constexpr disjoint_sparse_table() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr explicit disjoint_sparse_table(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        log = std::max<size_type>(1, std::countr_zero(std::bit_ceil(len)));

        buf.build(std::views::iota(size_type{0}, log) |
                  std::views::transform([this](auto) { return len; }));

        std::ranges::copy(r, buf[0].begin());
        for (size_type lvl = 1; lvl < log; ++lvl) {
            auto &&cur = buf[lvl];
            std::ranges::copy(buf[0], cur.begin());

            for (size_type k = 1 << lvl, j = k; j <= len; j += 2 * k) {
                for (size_type p = j - 1, u = j - k; p >= u + 1; --p)
                    cur[p - 1] = cur[p - 1] + cur[p];
                for (size_type p = j, v = std::min(len, j + k); p < v - 1; ++p)
                    cur[p + 1] = cur[p] + cur[p + 1];
            }
        }
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);
        if (l + 1 == r) return buf[0][l];

        --r;
        auto &&cur = buf[std::bit_width(l ^ r) - 1];
        return cur[l] + cur[r];
    }
};
} // namespace mld

#endif // MALLARD_DISJOINT_SPARSE_TABLE_H
