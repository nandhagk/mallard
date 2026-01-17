#ifndef MALLARD_INTERNAL_SPARSE_LAZY_SEGMENT_TREE_H
#define MALLARD_INTERNAL_SPARSE_LAZY_SEGMENT_TREE_H 1

#include <concepts>
#include <limits>
#include <memory>

#include "lib/actions/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::internal {
template <std::integral I, actions::internal::action Action, typename Manager>
struct sparse_lazy_segment_tree {
    using index_type = I;
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

    [[nodiscard]] static constexpr usize diff(index_type l, index_type r) {
        return static_cast<usize>(r - l);
    }

public:
    struct node {
        pointer l = handler::nil, r = handler::nil;
        index_type lx, rx, Lx, Rx;
        operand val, sum, sub;
        operation lzy;
        i32 h;

        constexpr node() noexcept
            : lx(std::numeric_limits<index_type>::max()),
              rx(std::numeric_limits<index_type>::lowest()),
              Lx(lx),
              Rx(rx) {}

        constexpr node(index_type p, index_type q, const operand &x) noexcept
            : lx(p), rx(q), Lx(p), Rx(q), val(x), sum(x * diff(lx, rx)), sub(sum) {}

        [[nodiscard]] constexpr i32 factor() const noexcept {
            return l->h - r->h;
        }

        constexpr void pull() noexcept {
            h = std::max(l->h, r->h) + 1;
            Lx = std::min(lx, l->Lx);
            Rx = std::max(rx, r->Rx);
            sub = l->sub + sum + r->sub;
        }

        constexpr void push() noexcept {
            if (lzy) {
                if (l != handler::nil) l->apply(lzy);
                if (r != handler::nil) r->apply(lzy);
                lzy = operation();
            }
        }

        constexpr void apply(const operation &f) noexcept {
            lzy = lzy + f;
            val = action::act(f, val);
            sum = action::act(action::pow(f, diff(lx, rx)), sum);
            sub = action::act(action::pow(f, diff(Lx, Rx)), sub);
        }
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    [[nodiscard]] static constexpr pointer rotate_right(pointer v) noexcept {
        pointer l = v->l;
        v->l = l->r;
        l->r = v;
        v->pull();
        l->pull();
        return l;
    }

    [[nodiscard]] static constexpr pointer rotate_left(pointer v) noexcept {
        pointer r = v->r;
        v->r = r->l;
        r->l = v;
        v->pull();
        r->pull();
        return r;
    }

    [[nodiscard]] static constexpr pointer balance(pointer v) noexcept {
        if (i32 bf = v->factor(); bf == 2) {
            if (v->l->factor() == -1) {
                v->l = rotate_left(v->l);
                v->pull();
            }
            return rotate_right(v);
        } else if (bf == -2) {
            if (v->r->factor() == 1) {
                v->r = rotate_right(v->r);
                v->pull();
            }
            return rotate_left(v);
        }
        return v;
    }

    [[nodiscard]] static constexpr pointer insert_leftmost(pointer v,
                                                           pointer u) noexcept {
        if (v == handler::nil) return u;
        v->push();
        v->l = insert_leftmost(v->l, u);
        v->pull();
        return balance(v);
    }

    [[nodiscard]] static constexpr pointer insert_rightmost(pointer v,
                                                            pointer u) noexcept {
        if (v == handler::nil) return u;
        v->push();
        v->r = insert_rightmost(v->r, u);
        v->pull();
        return balance(v);
    }

    [[nodiscard]] constexpr std::pair<pointer, pointer> split(pointer v, index_type l,
                                                              index_type r) noexcept {
        l = std::max(l, v->lx), r = std::min(r, v->rx);
        assert(l < r);

        pointer a = handler::nil, c = handler::nil;
        if (v->lx < l) a = hlr.create(v->lx, l, v->val);
        if (r < v->rx) c = hlr.create(r, v->rx, v->val);

        v->lx = l, v->rx = r, v->sum = v->val * diff(l, r);
        return {a, c};
    }

public:
    explicit constexpr sparse_lazy_segment_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {}

    [[nodiscard]] constexpr operand get(pointer v, index_type k) noexcept {
        for (; v != handler::nil;) {
            v->push();
            if (k < v->lx)
                v = v->l;
            else if (v->rx <= k)
                v = v->r;
            else
                return v->val;
        }
    }

    [[nodiscard]] constexpr pointer set(pointer v, index_type k,
                                        const operand &x) noexcept {
        if (v == handler::nil || k < v->Lx || v->Rx <= k) return v;

        v->push();
        if (k < v->lx) {
            v->l = set(v->l, k, x);
        } else if (v->rx <= k) {
            v->r = set(v->r, k, x);
        } else {
            auto [a, c] = split(v, k, k + 1);
            if (a != handler::nil) v->l = insert_rightmost(v->l, a);
            if (c != handler::nil) v->r = insert_leftmost(v->r, c);
            v->sum = v->val = x;
        }

        v->pull();
        return balance(v);
    }

    [[nodiscard]] constexpr operand fold(pointer v, index_type l,
                                         index_type r) noexcept {
        if (v == handler::nil || r <= v->Lx || v->Rx <= l) return operand();
        if (l <= v->Lx && v->Rx <= r) return v->sub;

        v->push();
        operand x = fold(v->l, l, r);
        if (index_type L = std::max(l, v->lx), R = std::min(r, v->rx); L < R) {
            if (l <= v->lx && v->rx <= r)
                x = x + v->sum;
            else
                x = x + (v->val * diff(L, R));
        }
        return x + fold(v->r, l, r);
    }

    [[nodiscard]] constexpr pointer apply(pointer v, index_type l, index_type r,
                                          const operation &f) noexcept {
        if (v == handler::nil || r <= v->Lx || v->Rx <= l) return v;
        if (l <= v->Lx && v->Rx <= r) {
            v->apply(f);
            return v;
        }

        v->push();
        v->l = apply(v->l, l, r, f), v->r = apply(v->r, l, r, f);
        if (index_type L = std::max(l, v->lx), R = std::min(r, v->rx); L < R) {
            if (L != v->lx || R != v->rx) {
                auto [a, c] = split(v, l, r);
                if (a != handler::nil) v->l = insert_rightmost(v->l, a);
                if (c != handler::nil) v->r = insert_leftmost(v->r, c);
            }
            v->val = action::act(f, v->val);
            v->sum = action::act(action::pow(f, diff(v->lx, v->rx)), v->sum);
        }
        v->pull();
        return balance(v);
    }
};
} // namespace mld::internal

#endif // MALLARD_SPARSE_LAZY_SEGMENT_TREE_H
