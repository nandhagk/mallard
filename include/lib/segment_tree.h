#ifndef MALLARD_SEGMENT_TREE_H
#define MALLARD_SEGMENT_TREE_H 1

#include <algorithm>
#include <bit>
#include <cassert>
#include <numeric>
#include <ranges>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

template <algebra::internal::monoid Monoid>
struct segment_tree {
    using operand = Monoid;

    using size_type = u32;
    using container_type = std::vector<operand>;

private:
    size_type len, cap;
    container_type buf;

    constexpr void pull(size_type k) noexcept {
        buf[k] = buf[k * 2] + buf[k * 2 + 1];
    }

public:
    constexpr segment_tree() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr segment_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        cap = std::bit_ceil(len);
        buf.assign(2 * cap, operand());

        std::ranges::copy(r, buf.begin() + cap);
        for (size_type i = cap - 1; i >= 1; --i) pull(i);
    }

    [[nodiscard]] constexpr operand get(size_type k) noexcept {
        assert(k < len);
        return buf[k + cap];
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(k < len);
        for (k += cap, buf[k] = x; k >>= 1; pull(k));
    }

    constexpr void add(size_type k, const operand &x) noexcept {
        assert(k < len);
        for (k += cap, buf[k] = buf[k] + x; k >>= 1; pull(k));
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);

        operand xl, xr;
        for (l += cap, r += cap; l < r; l >>= 1, r >>= 1) {
            if (l & 1) xl = xl + buf[l++];
            if (r & 1) xr = buf[--r] + xr;
        }
        return xl + xr;
    }

    [[nodiscard]] constexpr operand fold() const noexcept {
        return buf[1];
    }

    template <std::convertible_to<std::function<bool(operand)>> F>
    [[nodiscard]] constexpr size_type max_right(size_type l, F &&f) const noexcept {
        assert(l < len);
        assert(f(operand()));

        l += cap;
        operand x;
        do {
            l >>= std::countr_zero(l);
            if (!f(x + buf[l])) {
                while (l < cap) {
                    l = 2 * l;
                    if (operand y = x + buf[l]; f(y)) x = y, ++l;
                }

                return l - cap;
            }

            x = x + buf[l++];
        } while ((l & -l) != l);

        return len;
    }

    template <std::convertible_to<std::function<bool(operand)>> F>
    [[nodiscard]] constexpr size_type min_left(size_type r, F &&f) const noexcept {
        assert(0 < r && r <= len);
        assert(f(operand()));

        r += cap;
        operand x;
        do {
            for (--r; r > 1 && (r % 2); r >>= 1);
            if (!f(buf[r] + x)) {
                while (r < cap) {
                    r = 2 * r + 1;
                    if (operand y = buf[r] + x; f(y)) x = y, --r;
                }

                return r + 1 - cap;
            }

            x = buf[r] + x;
        } while ((r & -r) != r);

        return 0;
    }
};
} // namespace mld

#endif // MALLARD_SEGMENT_TREE_H
