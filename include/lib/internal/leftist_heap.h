#ifndef MALLARD_INTERNAL_LEFTIST_HEAP_H
#define MALLARD_INTERNAL_LEFTIST_HEAP_H 1

#include <algorithm>
#include <functional>

#include "lib/prelude.h"

namespace mld::internal {
template <typename Key, typename Value, typename Manager,
          typename Compare = std::less<Key>>
struct leftist_heap {
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
    constexpr leftist_heap(const comparator &compare = comparator(),
                           const allocator_type &alloc = allocator_type()) noexcept
        : cmp(compare), hlr(alloc) {}

    explicit constexpr leftist_heap(const allocator_type &alloc) noexcept
        : hlr(alloc) {}

    struct node {
        pointer l = handler::nil, r = handler::nil;
        size_type rnk;
        key_type key;
        value_type val;

        constexpr node() noexcept = default;

        constexpr node(const key_type &k, const value_type &v) noexcept
            : rnk(1), key(k), val(v) {}
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    constexpr void meld(pointer &a, pointer b) noexcept {
        if (b == handler::nil) return;
        if (a == handler::nil) return a = b, void();

        if (cmp(a->key, b->key)) std::swap(a, b);

        a = hlr.clone(a);
        meld(a->r, b);

        if (a->r->rnk > a->l->rnk) std::swap(a->l, a->r);
        a->rnk = a->r->rnk + 1;
    }

    constexpr void insert(pointer &a, const key_type &k, const value_type &v) noexcept {
        meld(a, hlr.create(k, v));
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_LEFTIST_HEAP_H
