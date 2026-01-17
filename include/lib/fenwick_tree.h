#ifndef MALLARD_FENWICK_TREE_H
#define MALLARD_FENWICK_TREE_H 1

#include <algorithm>
#include <cassert>
#include <numeric>
#include <ranges>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

template <algebra::internal::abelian_monoid Monoid>
struct fenwick_tree {
    using operand = Monoid;

    using size_type = u32;
    using container_type = std::vector<operand>;

private:
    size_type len;
    container_type buf;

public:
    constexpr fenwick_tree() = default;

    explicit constexpr fenwick_tree(size_type n) noexcept {
        build(n);
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr fenwick_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        buf.assign(n, operand());
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        buf.assign(std::ranges::begin(r), std::ranges::end(r));

        for (size_type i = 1; i <= len; ++i)
            if (size_type j = i + (i & -i); j <= len)
                buf[j - 1] = buf[i - 1] + buf[j - 1];
    }

    constexpr void add(size_type k, const operand &x) noexcept {
        assert(k < len);
        for (++k; k <= len; k += k & -k) buf[k - 1] = buf[k - 1] + x;
    }

    [[nodiscard]] constexpr operand fold(size_type r) const noexcept {
        assert(r <= len);

        operand x;
        for (; r != 0; r -= r & -r) x = x + buf[r - 1];
        return x;
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept
        requires algebra::internal::invertible<operand>
    {
        if (l == 0) return fold(r);
        assert(l <= r && r <= len);

        operand xl, xr;
        for (; l < r; r -= r & -r) xr = xr + buf[r - 1];
        for (; r < l; l -= l & -l) xl = xl + buf[l - 1];
        return -xl + xr;
    }
};
} // namespace mld

#endif // MALLARD_FENWICK_TREE_H
