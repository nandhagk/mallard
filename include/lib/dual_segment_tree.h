#ifndef MALLARD_DUAL_SEGMENT_TREE_H
#define MALLARD_DUAL_SEGMENT_TREE_H 1

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

#include "lib/actions/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <actions::internal::action Action>
struct dual_segment_tree {
    using action = Action;

    using operand = action::operand;
    using operation = action::operation;

    using size_type = u32;

private:
    size_type len, cap, log;
    std::vector<operand> buf;
    std::vector<operation> lzy;

    constexpr void apply_at(size_type k, const operation &f) noexcept {
        if (k < cap)
            lzy[k] = lzy[k] + f;
        else
            buf[k - cap] = action::act(f, buf[k - cap]);
    }

    constexpr void push(size_type k) noexcept {
        if (lzy[k]) {
            apply_at(k * 2, lzy[k]);
            apply_at(k * 2 + 1, lzy[k]);
            lzy[k] = operation();
        }
    }

public:
    constexpr dual_segment_tree() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr explicit dual_segment_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = std::ranges::size(r);
        cap = std::bit_ceil(len);
        log = std::countr_zero(cap);
        buf.assign(cap, operand());
        lzy.assign(cap, operation());

        std::ranges::copy(r, buf.begin());
    }

    [[nodiscard]] constexpr operand get(size_type k) const noexcept {
        assert(k < len);

        operand x = buf[k];
        for (k += cap; k >>= 1; x = action::act(lzy[k], x));

        return x;
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(k < len);

        k += cap;
        for (size_type i = log; i >= 1; --i) push(k >> i);

        buf[k - cap] = x;
    }

    constexpr void add(size_type k, const operand &x) noexcept {
        assert(k < len);

        k += cap;
        for (size_type i = log; i >= 1; --i) push(k >> i);

        buf[k - cap] = buf[k - cap] + x;
    }

    constexpr void apply(size_type k, const operation &f) noexcept {
        assert(k < len);

        k += cap;
        for (size_type i = log; i >= 1; --i) push(k >> i);

        buf[k - cap] = action::act(f, buf[k - cap]);
    }

    constexpr void apply(size_type l, size_type r, const operation &f) noexcept {
        assert(l < r && r <= len);

        l += cap, r += cap;
        for (size_type i = log; i >= 1; --i) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }

        for (size_type p = l, q = r; p < q; p >>= 1, q >>= 1) {
            if (p & 1) apply_at(p++, f);
            if (q & 1) apply_at(--q, f);
        }
    }
};
} // namespace mld

#endif // MALLARD_DUAL_SEGMENT_TREE_H
