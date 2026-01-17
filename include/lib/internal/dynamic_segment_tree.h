#ifndef MALLARD_INTERNAL_DYNAMIC_SEGMENT_TREE_H
#define MALLARD_INTERNAL_DYNAMIC_SEGMENT_TREE_H 1

#include <concepts>
#include <iterator>
#include <numeric>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::internal {
template <algebra::internal::monoid Monoid, typename Manager>
struct dynamic_segment_tree {
    using operand = Monoid;

    struct node;
    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using size_type = u32;
    using allocator_type = typename handler::allocator_type;

protected:
    [[no_unique_address]] handler hlr;

public:
    explicit constexpr dynamic_segment_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {}

    struct node {
        pointer l = handler::nil, r = handler::nil;
        operand val;

        constexpr node() noexcept = default;

        explicit constexpr node(const operand &x) noexcept
            : val(x) {}

        constexpr void pull() noexcept {
            val = l->val + r->val;
        }
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    template <std::input_iterator InputIt>
    [[nodiscard]] constexpr pointer build(InputIt &it, size_type s,
                                          size_type e) noexcept {
        if (e - s == 1) return hlr.create(*(it++));
        pointer v = hlr.create();

        size_type m = std::midpoint(s, e);
        v->l = build(it, s, m);
        v->r = build(it, m, e);

        v->pull();
        return v;
    }

    [[nodiscard]] constexpr operand get(const pointer &v, size_type k, size_type s,
                                        size_type e) const noexcept {
        if (e - s == 1) return v->val;

        if (size_type m = std::midpoint(s, e); k < m)
            return get(v->l, k, s, m);
        else
            return get(v->r, k, m, e);
    }

    constexpr void set(pointer &v, size_type k, const operand &x, size_type s,
                       size_type e) noexcept {
        v = hlr.clone(v);
        if (e - s == 1) {
            v->val = x;
            return;
        }

        if (size_type m = std::midpoint(s, e); k < m)
            set(v->l, k, x, s, m);
        else
            set(v->r, k, x, m, e);

        v->pull();
    }

    constexpr void add(pointer &v, size_type k, const operand &x, size_type s,
                       size_type e) noexcept {
        v = hlr.clone(v);
        if (e - s == 1) {
            v->val = v->val + x;
            return;
        }

        if (size_type m = std::midpoint(s, e); k < m)
            add(v->l, k, x, s, m);
        else
            add(v->r, k, x, m, e);

        v->pull();
    }

    [[nodiscard]] constexpr operand fold(const pointer &v, size_type l, size_type r,
                                         size_type s, size_type e) const noexcept {
        if (l <= s && e <= r) return v->val;

        size_type m = std::midpoint(s, e);
        if (r <= m) return fold(v->l, l, r, s, m);
        if (m <= l) return fold(v->r, l, r, m, e);
        return fold(v->l, l, r, s, m) + fold(v->r, l, r, m, e);
    }

    constexpr void copy(pointer &u, const pointer &v, size_type l, size_type r,
                        size_type s, size_type e) noexcept {
        if (l <= s && e <= r) {
            u = v;
            return;
        }
        if (r <= s || e <= l) return;

        u = hlr.clone(u);
        size_type m = std::midpoint(s, e);
        copy(u->l, v->l, l, r, s, m);
        copy(u->r, v->r, l, r, m, e);
        u->pull();
    }

    // TODO: Test this
    template <std::convertible_to<std::function<bool(operand)>> F>
    constexpr size_type max_right(const pointer &v, size_type l, F &&f, size_type s,
                                  size_type e, operand &x) const noexcept {
        if (e <= l) return e;
        if (l <= s) {
            if (operand y = x + v->val; f(y)) return x = std::move(y), e;
            if (e - s == 1) return s;
        }

        size_type m = std::midpoint(s, e);
        if (size_type k = max_right(v->l, l, std::forward<F>(f), s, m, x); k < m)
            return k;
        return max_right(v->r, l, std::forward<F>(f), m, e, x);
    }

    template <typename F>
    constexpr size_type min_left(const pointer &v, size_type r, F &&f, size_type s,
                                 size_type e, operand &x) const noexcept {
        if (r <= s) return s;
        if (e <= r) {
            if (operand y = v->val + x; f(y)) return x = std::move(y), s;
            if (e - s == 1) return e;
        }

        size_type m = std::midpoint(s, e);
        if (size_type k = min_left(v->r, r, std::forward<F>(f), m, e, x); k > m)
            return k;
        return min_left(v->l, r, std::forward<F>(f), s, m, x);
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_DYNAMIC_SEGMENT_TREE_H
