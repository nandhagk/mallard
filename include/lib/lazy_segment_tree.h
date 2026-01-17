#ifndef MALLARD_LAZY_SEGMENT_TREE_H
#define MALLARD_LAZY_SEGMENT_TREE_H 1

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

#include "lib/actions/internal/concepts.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <actions::internal::action Action>
struct lazy_segment_tree {
    using action = Action;

    using operand = action::operand;
    using operation = action::operation;

    using size_type = u32;

private:
    size_type len, cap, log;
    std::vector<operand> buf;
    std::vector<operation> lzy;

    constexpr void pull(size_type k) noexcept {
        buf[k] = buf[k * 2] + buf[k * 2 + 1];
    }

    constexpr void pull_with_lazy(size_type k) noexcept {
        size_type sz = 1 << (log - (std::bit_width(k) - 1));
        buf[k] = action::act(action::pow(lzy[k], sz), buf[k * 2] + buf[2 * k + 1]);
    }

    constexpr void apply_at(size_type k, const operation &f) noexcept {
        size_type sz = 1 << (log - (std::bit_width(k) - 1));
        buf[k] = action::act(action::pow(f, sz), buf[k]);
        if (k < cap) lzy[k] = lzy[k] + f;
    }

    constexpr void push(size_type k) noexcept {
        if (lzy[k]) {
            apply_at(k * 2, lzy[k]);
            apply_at(k * 2 + 1, lzy[k]);
            lzy[k] = operation();
        }
    }

public:
    constexpr lazy_segment_tree() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit lazy_segment_tree(R &&r) {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        cap = std::bit_ceil(len);
        log = std::countr_zero(cap);
        buf.assign(2 * cap, operand());
        lzy.assign(cap, operation());

        std::ranges::copy(r, buf.begin() + cap);
        for (size_type i = cap - 1; i >= 1; --i) pull(i);
    }

    [[nodiscard]] constexpr operand get(size_type k) noexcept {
        assert(k < len);

        operand x = buf[k += cap];
        for (; k >>= 1; x = action::act(lzy[k], x));

        return x;
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(k < len);

        k += cap;
        for (size_type i = log; i >= 1; --i) push(k >> i);

        for (buf[k] = x; k >>= 1; pull(k));
    }

    constexpr void add(size_type k, const operand &x) noexcept {
        assert(k < len);

        k += cap;
        for (size_type i = log; i >= 1; --i) push(k >> i);

        for (buf[k] = buf[k] + x; k >>= 1; pull(k));
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) noexcept {
        assert(l < r && r <= len);

        l += cap, r += cap;
        size_type i = 1, a = 0, b = 0, p = l, q = r;

        operand xl, xr;
        for (; l < r; ++i, l >>= 1, r >>= 1) {
            if (l & 1) xl = xl + buf[l++], a += 1 << (i - 1);
            if (r & 1) xr = buf[--r] + xr, b += 1 << (i - 1);
            if (((p >> i) << i) != p) xl = action::act(action::pow(lzy[p >> i], a), xl);
            if (((q >> i) << i) != q)
                xr = action::act(action::pow(lzy[(q - 1) >> i], b), xr);
        }

        for (; i <= log; ++i) {
            if (((p >> i) << i) != p) xl = action::act(action::pow(lzy[p >> i], a), xl);
            if (((q >> i) << i) != q)
                xr = action::act(action::pow(lzy[(q - 1) >> i], b), xr);
        }

        return xl + xr;
    }

    [[nodiscard]] constexpr operand fold() const noexcept {
        return buf[1];
    }

    constexpr void apply(size_type k, const operation &f) noexcept {
        assert(k < len);

        k += cap;
        for (size_type i = log; i >= 1; --i) push(k >> i);

        for (buf[k] = action::act(f, buf[k]); k >>= 1; pull(k));
    }

    constexpr void apply(size_type l, size_type r, const operation &f) noexcept {
        assert(l < r && r <= len);

        l += cap, r += cap;
        if constexpr (algebra::internal::commutative<operation>) {
            size_type i = 1, p = l, q = r;
            for (; l < r; ++i, l >>= 1, r >>= 1) {
                if (l & 1) apply_at(l++, f);
                if (r & 1) apply_at(--r, f);
                pull_with_lazy(p >> i);
                pull_with_lazy((q - 1) >> i);
            }

            for (; i <= log; i++) {
                pull_with_lazy(p >> i);
                pull_with_lazy((q - 1) >> i);
            }
        } else {
            for (size_type i = log; i >= 1; --i) {
                if (((l >> i) << i) != l) push(l >> i);
                if (((r >> i) << i) != r) push((r - 1) >> i);
            }

            for (size_type p = l, q = r; p < q; p >>= 1, q >>= 1) {
                if (p & 1) apply_at(p++, f);
                if (q & 1) apply_at(--q, f);
            }

            for (size_type i = 1; i <= log; ++i) {
                if (((l >> i) << i) != l) pull(l >> i);
                if (((r >> i) << i) != r) pull((r - 1) >> i);
            }
        }
    }

    template <std::convertible_to<std::function<bool(operand)>> F>
    [[nodiscard]] constexpr size_type max_right(size_type l, F &&f) noexcept {
        assert(l < len);
        assert(f(operand()));

        l += cap;
        for (size_type i = log; i >= 1; --i) push(l >> i);

        operand x;
        do {
            l >>= std::countr_zero(l);
            if (!f(x + buf[l])) {
                while (l < cap) {
                    push(l), l = 2 * l;
                    if (operand y = x + buf[l]; f(y)) x = y, ++l;
                }

                return l - cap;
            }

            x = x + buf[l++];
        } while ((l & -l) != l);

        return len;
    }

    template <std::convertible_to<std::function<bool(operand)>> F>
    [[nodiscard]] constexpr size_type min_left(size_type r, F &&f) noexcept {
        assert(0 < r && r <= len);
        assert(f(operand()));

        r += cap;
        for (size_type i = log; i >= 1; --i) push((r - 1) >> i);

        operand x;
        do {
            for (--r; r > 1 && (r % 2); r >>= 1);
            if (!f(buf[r] + x)) {
                while (r < cap) {
                    push(r), r = 2 * r + 1;
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

#endif // MALLARD_LAZY_SEGMENT_TREE_H
