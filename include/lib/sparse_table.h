#ifndef MALLARD_SPARSE_TABLE_H
#define MALLARD_SPARSE_TABLE_H 1

#include <algorithm>
#include <cassert>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

namespace mld {
template <algebra::internal::semigroup Semigroup>
    requires algebra::internal::idempotent<Semigroup>
struct sparse_table {
    using operand = Semigroup;

    using size_type = u32;
    using container_type = static_csr<operand>;

private:
    size_type len, log;
    container_type buf;

public:
    constexpr sparse_table() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit sparse_table(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        log = std::max<size_type>(1, std::countr_zero(std::bit_ceil(len)));

        buf.build(
            std::views::iota(size_type{0}, log) |
            std::views::transform([this](auto lvl) { return len - ((1 << lvl) - 1); }));

        std::ranges::copy(r, buf[0].begin());
        for (size_type lvl = 0; lvl < log - 1; ++lvl) {
            auto &&prv = buf[lvl];
            auto &&cur = buf[lvl + 1];
            std::ranges::transform(
                prv, std::span(prv.data() + (1 << lvl), prv.data() + prv.size()),
                cur.begin(), [](const operand &a, const operand &b) { return a + b; });
        }
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);
        if (l + 1 == r) return buf[0][l];

        size_type lvl = std::bit_width(r - l - 1) - 1;
        auto &&cur = buf[lvl];
        return cur[l] + cur[r - (1 << lvl)];
    }
};
} // namespace mld

#endif // MALLARD_SPARSE_TABLE_H
