#ifndef MALLARD_LICHAO_TREE_H
#define MALLARD_LICHAO_TREE_H 1

#include <algorithm>
#include <limits>
#include <memory>
#include <numeric>

#include "lib/internal/lichao_tree.h"
#include "lib/prelude.h"

namespace mld {
template <std::integral I, typename Line, typename Manager>
struct lichao_tree : private internal::lichao_tree<I, Line, Manager> {

private:
    using base = internal::lichao_tree<I, Line, Manager>;

public:
    using typename base::index_type;
    using typename base::line;

    using typename base::handler;
    using typename base::node;
    using typename base::pointer;

    using typename base::allocator_type;
    using typename base::size_type;

private:
    index_type lo, hi;
    pointer root = handler::nil;

public:
    constexpr ~lichao_tree() noexcept {
        base::dispose(root);
    }

    constexpr lichao_tree(const allocator_type &alloc = allocator_type()) noexcept
        : lichao_tree(std::numeric_limits<index_type>::lowest(),
                      std::numeric_limits<index_type>::max(), alloc) {}

    constexpr lichao_tree(index_type l, index_type r,
                          const allocator_type &alloc = allocator_type()) noexcept
        : base(alloc), lo(l), hi(r) {}

    constexpr void add(line x) noexcept {
        base::add(root, x, lo, hi, x(lo), x(hi));
    }

    constexpr void add(index_type l, index_type r, line x) noexcept {
        base::add(root, x, l, r, lo, hi, x(lo), x(hi));
    }

    [[nodiscard]] constexpr line query(index_type k) const noexcept {
        assert(lo <= k && k < hi);
        return base::query(root, lo, hi, k);
    }
};
} // namespace mld

#endif // MALLARD_LICHAO_TREE_H
