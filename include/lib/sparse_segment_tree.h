#ifndef MALLARD_SPARSE_SEGMENT_TREE_H
#define MALLARD_SPARSE_SEGMENT_TREE_H 1

#include <concepts>
#include <limits>
#include <memory>
#include <numeric>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/internal/sparse_segment_tree.h"
#include "lib/prelude.h"

namespace mld {
template <std::integral I, algebra::internal::monoid Monoid, typename Manager>
struct sparse_segment_tree : private internal::sparse_segment_tree<I, Monoid, Manager> {
private:
    using base = internal::sparse_segment_tree<I, Monoid, Manager>;

public:
    using typename base::index_type;
    using typename base::operand;

    using typename base::handler;
    using typename base::node;
    using typename base::pointer;

    using typename base::allocator_type;
    using typename base::size_type;

private:
    index_type lo, hi;
    pointer root = handler::nil;

public:
    constexpr ~sparse_segment_tree() noexcept {
        base::dispose(root);
    }

    constexpr sparse_segment_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : sparse_segment_tree(std::numeric_limits<index_type>::lowest(),
                              std::numeric_limits<index_type>::max(), alloc) {}

    constexpr sparse_segment_tree(
        index_type l, index_type r,
        const allocator_type &alloc = allocator_type()) noexcept
        : base(alloc), lo(l), hi(r) {}

    constexpr sparse_segment_tree(const sparse_segment_tree &other,
                                  const allocator_type &alloc) noexcept
        : base(alloc), lo(other.lo), hi(other.hi), root(other.root) {}

    constexpr sparse_segment_tree(sparse_segment_tree &&other,
                                  const allocator_type &alloc) noexcept
        : base(alloc), lo(other.lo), hi(other.hi), root(other.root) {}

    [[nodiscard]] constexpr operand get(index_type k) const noexcept {
        assert(lo <= k && k < hi);
        return base::get(root, k, lo, hi);
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(lo <= k && k < hi);
        base::set(root, k, x, lo, hi);
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept {
        assert(lo <= l && l < r && r <= hi);
        return base::fold(root, l, r, lo, hi);
    }

    [[nodiscard]] constexpr operand fold() const noexcept {
        return root->sum;
    }
};
} // namespace mld

#endif // MALLARD_SPARSE_SEGMENT_TREE_H
