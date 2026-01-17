#ifndef MALLARD_INTERNAL_SKEW_HEAP_H
#define MALLARD_INTERNAL_SKEW_HEAP_H 1

#include <algorithm>
#include <functional>

#include "lib/prelude.h"

namespace mld::internal {
// TODO: Make this a monoid?
template <typename Key, typename Value, typename Manager,
          typename Compare = std::less<Key>>
struct skew_heap {
    using key_type = Key;
    using value_type = Value;

    struct node;
    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using size_type = u32;
    using allocator_type = typename handler::allocator_type;

    using comparator = Compare;

protected:
    [[no_unique_address]] comparator cmp;
    [[no_unique_address]] handler hlr;

public:
    constexpr skew_heap(const comparator &compare = comparator(),
                        const allocator_type &alloc = allocator_type()) noexcept
        : cmp(compare), hlr(alloc) {}

    explicit constexpr skew_heap(const allocator_type &alloc) noexcept
        : hlr(alloc) {}

    struct node {
        pointer l = handler::nil, r = handler::nil;
        size_type rnk;
        key_type key, lzy;
        value_type val;

        constexpr node() noexcept = default;

        constexpr node(const key_type &k, const value_type &v) noexcept
            : rnk(1), key(k), val(v) {}

        constexpr void apply_at(const key_type &z) noexcept {
            lzy += z;
            push();
        }

        constexpr void push() noexcept {
            if (lzy) {
                if (l != handler::nil) l->lzy += lzy;
                if (r != handler::nil) r->lzy += lzy;
                key += lzy;
                lzy = 0;
            }
        }
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    [[nodiscard]] constexpr pointer meld(pointer a, pointer b) noexcept {
        if (b == handler::nil) return a->push(), a;
        if (a == handler::nil) return b->push(), b;

        a->push(), b->push();
        if (cmp(a->key, b->key)) std::swap(a, b);

        std::swap(a->l, a->r = meld(b, a->r));
        return a;
    }

    [[nodiscard]] constexpr pointer insert(pointer a, const key_type &k,
                                           const value_type &v) noexcept {
        return meld(a, hlr.create(k, v));
    }

    [[nodiscard]] constexpr pointer erase(pointer a) noexcept {
        a->push();
        pointer b = meld(a->l, a->r);
        return hlr.dispose(a), b;
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_SKEW_HEAP_H
