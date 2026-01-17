#ifndef MALLARD_INTERNAL_DYNAMIC_LAZY_SEGMENT_TREE_H
#define MALLARD_INTERNAL_DYNAMIC_LAZY_SEGMENT_TREE_H 1

#include <iterator>
#include <numeric>

#include "lib/actions/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::internal {
template <actions::internal::action Action, typename Manager>
struct dynamic_lazy_segment_tree {
    using action = Action;

    using operand = action::operand;
    using operation = action::operation;

    struct node;
    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using size_type = u32;
    using allocator_type = typename handler::allocator_type;

protected:
    [[no_unique_address]] handler hlr;

    constexpr void apply_at(pointer &v, const operation &f, usize sz) noexcept {
        v = hlr.clone(v);
        v->apply(f, sz);
    }

public:
    explicit constexpr dynamic_lazy_segment_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {}

    struct node {
        pointer l = handler::nil, r = handler::nil;
        operand val;
        operation lzy;

        constexpr node() noexcept = default;

        explicit constexpr node(const operand &x) noexcept
            : val(x) {}

        constexpr void pull() noexcept {
            val = l->val + r->val;
            lzy = operation();
        }

        constexpr void apply(const operation &f, usize sz) noexcept {
            val = action::act(action::pow(f, sz), val);
            lzy = lzy + f;
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
                                        size_type e,
                                        const operation &p) const noexcept {
        if (e - s == 1) return action::act(p, v->val);

        operation q = v->lzy + p;
        if (size_type m = std::midpoint(s, e); k < m)
            return get(v->l, k, s, m, q);
        else
            return get(v->r, k, m, e, q);
    }

    constexpr void set(pointer &v, size_type k, const operand &x, size_type s,
                       size_type e) noexcept {
        v = hlr.clone(v);
        if (e - s == 1) {
            v->val = x;
            return;
        }

        size_type m = std::midpoint(s, e);
        apply_at(v->l, v->lzy, m - s);
        apply_at(v->r, v->lzy, e - m);
        if (k < m)
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

        size_type m = std::midpoint(s, e);
        apply_at(v->l, v->lzy, m - s);
        apply_at(v->r, v->lzy, e - m);
        if (k < m)
            add(v->l, k, x, s, m);
        else
            add(v->r, k, x, m, e);

        v->pull();
    }

    [[nodiscard]] constexpr operand fold(const pointer &v, size_type l, size_type r,
                                         size_type s, size_type e,
                                         const operation &p) const noexcept {
        if (l <= s && e <= r) return action::act(action::pow(p, e - s), v->val);

        operation q = v->lzy + p;
        size_type m = std::midpoint(s, e);
        if (r <= m) return fold(v->l, l, r, s, m, q);
        if (m <= l) return fold(v->r, l, r, m, e, q);
        return fold(v->l, l, r, s, m, q) + fold(v->r, l, r, m, e, q);
    }

    constexpr void copy(pointer &u, const pointer &v, size_type l, size_type r,
                        size_type s, size_type e, const operation &p,
                        const operation &y) noexcept {
        if (l <= s && e <= r) return apply_at(u = v, y, e - s);
        if (r <= s || e <= l) return apply_at(u, p, e - s);

        operation q = u->lzy + p, z = v->lzy + y;
        size_type m = std::midpoint(s, e);

        u = hlr.clone(u);
        copy(u->l, v->l, l, r, s, m, q, z);
        copy(u->r, v->r, l, r, m, e, q, z);
        u->pull();
    }

    constexpr void apply(pointer &v, size_type l, size_type r, const operation &f,
                         size_type s, size_type e, const operation &p) noexcept {
        if (l <= s && e <= r) return apply_at(v, p + f, e - s);
        if (r <= s || e <= l) return apply_at(v, p, e - s);

        operation q = v->lzy + p;
        size_type m = std::midpoint(s, e);

        v = hlr.clone(v);
        apply(v->l, l, r, f, s, m, q);
        apply(v->r, l, r, f, m, e, q);
        v->pull();
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_DYNAMIC_LAZY_SEGMENT_TREE_H
