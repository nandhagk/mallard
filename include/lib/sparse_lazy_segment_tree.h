#ifndef MALLARD_SPARSE_LAZY_SEGMENT_TREE_H
#define MALLARD_SPARSE_LAZY_SEGMENT_TREE_H 1

#include <concepts>
#include <limits>
#include <memory>

#include "lib/actions/internal/concepts.h"
#include "lib/internal/sparse_lazy_segment_tree.h"
#include "lib/prelude.h"

namespace mld {
template <std::integral I, actions::internal::action Action, typename Manager>
struct sparse_lazy_segment_tree
    : private internal::sparse_lazy_segment_tree<I, Action, Manager> {

private:
    using base = internal::sparse_lazy_segment_tree<I, Action, Manager>;

public:
    using typename base::index_type;

    using typename base::action;
    using typename base::operand;
    using typename base::operation;

    using typename base::handler;
    using typename base::node;
    using typename base::pointer;

    using typename base::allocator_type;
    using typename base::size_type;

private:
    index_type lo, hi;
    pointer root = handler::nil;

public:
    constexpr ~sparse_lazy_segment_tree() noexcept {
        base::dispose(root);
    }

    constexpr sparse_lazy_segment_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : sparse_lazy_segment_tree(std::numeric_limits<index_type>::lowest(),
                                   std::numeric_limits<index_type>::max(), alloc) {}

    constexpr sparse_lazy_segment_tree(
        index_type l, index_type r,
        const allocator_type &alloc = allocator_type()) noexcept
        : base(alloc), lo(l), hi(r) {
        root = base::hlr.create(lo, hi, operand());
    }

    constexpr sparse_lazy_segment_tree(const sparse_lazy_segment_tree &other,
                                       const allocator_type &alloc) noexcept
        : base(alloc), lo(other.lo), hi(other.hi), root(other.root) {}

    constexpr sparse_lazy_segment_tree(sparse_lazy_segment_tree &&other,
                                       const allocator_type &alloc) noexcept
        : base(alloc), lo(other.lo), hi(other.hi), root(other.root) {}

    [[nodiscard]] constexpr operand get(index_type k) noexcept {
        assert(lo <= k && k < hi);
        return base::get(root, k);
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(lo <= k && k < hi);
        root = base::set(root, k, x);
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) noexcept {
        assert(lo <= l && l < r && r <= hi);
        return base::fold(root, l, r);
    }

    [[nodiscard]] constexpr operand fold() noexcept {
        return root->sub;
    }

    constexpr void apply(size_type l, size_type r, const operation &f) noexcept {
        assert(lo <= l && l < r && r <= hi);
        root = base::apply(root, l, r, f);
    }
};
} // namespace mld

#endif // MALLARD_SPARSE_LAZY_SEGMENT_TREE_H
