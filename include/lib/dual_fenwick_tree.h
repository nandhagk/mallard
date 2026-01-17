#ifndef MALLARD_DUAL_FENWICK_TREE_H
#define MALLARD_DUAL_FENWICK_TREE_H 1

#include "lib/actions/internal/concepts.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"

namespace mld {
template <actions::internal::action Action>
struct dual_fenwick_tree {
    using action = Action;

    using operand = action::operand;
    using operation = action::operation;

    static_assert(algebra::internal::abelian_group<operation>);

    using size_type = u32;

private:
    size_type len;
    std::vector<operand> buf;
    std::vector<operation> lzy;

public:
    constexpr dual_fenwick_tree() = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr explicit dual_fenwick_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        lzy.assign(len, operation());

        buf.resize(len);
        std::ranges::copy(r, buf.begin());
    }

    [[nodiscard]] constexpr operand get(size_type k) const noexcept {
        assert(k < len);

        operand x = buf[k];
        for (++k; k <= len; k += k & -k) x = action::act(lzy[k - 1], x);
        return x;
    }

    constexpr void apply(size_type l, size_type r, const operation &f) noexcept {
        assert(l < r && r <= len);

        operation g = -f;
        for (; l < r; r -= r & -r) lzy[r - 1] = f + lzy[r - 1];
        for (; r < l; l -= l & -l) lzy[l - 1] = g + lzy[l - 1];
    }
};
} // namespace mld

#endif // MALLARD_DUAL_FENWICK_TREE_H
