#ifndef MALLARD_SCAN_TABLE_H
#define MALLARD_SCAN_TABLE_H 1

#include <cassert>
#include <concepts>
#include <iterator>
#include <numeric>
#include <ranges>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

template <algebra::internal::monoid Monoid>
struct scan_table {
    using operand = Monoid;

    using size_type = u32;
    using container_type = std::vector<operand>;

private:
    size_type len;
    container_type buf;

public:
    constexpr scan_table() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr scan_table(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        buf.clear(), buf.reserve(len);

        std::inclusive_scan(
            std::ranges::begin(r), std::ranges::end(r), std::back_insert_iterator(buf),
            [](const operand &a, const operand &b) { return a + b; }, operand());
    }

    [[nodiscard]] constexpr operand get(size_type k) const noexcept {
        assert(k < len);
        return fold(k, k + 1);
    }

    [[nodiscard]] constexpr operand fold(size_type r) const noexcept {
        assert(0 < r && r <= len);
        return buf[--r];
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept
        requires algebra::internal::invertible<operand>
    {
        if (l == 0) return fold(r);

        assert(l < r && r <= len);
        return -buf[--l] + buf[--r];
    }
};

} // namespace mld

#endif // MALLARD_SCAN_TABLE_H
