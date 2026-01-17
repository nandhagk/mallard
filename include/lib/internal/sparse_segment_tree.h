#ifndef MALLARD_INTERNAL_SPARSE_SEGMENT_TREE_H
#define MALLARD_INTERNAL_SPARSE_SEGMENT_TREE_H 1

#include <concepts>
#include <limits>
#include <memory>
#include <numeric>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::internal {
template <std::integral I, algebra::internal::monoid Monoid, typename Manager>
struct sparse_segment_tree {
    using index_type = I;

    using operand = Monoid;

    struct node;
    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using size_type = u32;
    using allocator_type = typename handler::allocator_type;

protected:
    [[no_unique_address]] handler hlr;

public:
    explicit constexpr sparse_segment_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {}

    struct node {
        pointer l = handler::nil, r = handler::nil;
        index_type idx;
        operand val, sum;

        constexpr node() noexcept = default;

        constexpr node(index_type k, const operand &x) noexcept
            : idx(k), val(x), sum(x) {}

        constexpr void pull() noexcept {
            sum = l->sum + val + r->sum;
        }
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    [[nodiscard]] constexpr operand get(const pointer &u, index_type k, index_type l,
                                        index_type r) const noexcept {
        for (pointer v = u; v != handler::nil;) {
            if (v->idx == k) return v->val;

            if (index_type m = std::midpoint(l, r); k < m)
                v = v->l;
            else
                v = v->r;
        }

        return operand();
    }

    constexpr void set(pointer &v, index_type k, operand x, index_type s,
                       index_type e) noexcept {
        if (v == handler::nil) {
            v = hlr.create(k, x);
            return;
        }

        v = hlr.clone(v);
        if (v->idx == k) {
            v->val = x;
        } else if (index_type m = std::midpoint(s, e); k < m) {
            if (v->idx < k) {
                std::swap(v->idx, k);
                std::swap(v->val, x);
            }

            set(v->l, k, x, s, m);
        } else {
            if (v->idx > k) {
                std::swap(v->idx, k);
                std::swap(v->val, x);
            }

            set(v->r, k, x, m, e);
        }

        v->pull();
    }

    [[nodiscard]] constexpr operand fold(const pointer &v, index_type l, index_type r,
                                         index_type s, index_type e) const noexcept {
        if (v == handler::nil || r <= s || e <= l) return operand();
        if (l <= s && e <= r) return v->sum;

        index_type m = std::midpoint(s, e);

        operand x = fold(v->l, l, r, s, m);
        if (l <= v->idx && v->idx < r) x = x + v->val;
        return x + fold(v->r, l, r, m, e);
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_SPARSE_SEGMENT_TREE_H
