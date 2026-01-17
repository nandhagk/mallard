#ifndef MALLARD_DYNAMIC_LAZY_SEGMENT_TREE_H
#define MALLARD_DYNAMIC_LAZY_SEGMENT_TREE_H 1

#include <iterator>
#include <numeric>

#include "lib/actions/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/internal/dynamic_lazy_segment_tree.h"
#include "lib/prelude.h"

namespace mld {
template <actions::internal::action Action, typename Manager>
struct dynamic_lazy_segment_tree
    : private internal::dynamic_lazy_segment_tree<Action, Manager> {
private:
    using base = internal::dynamic_lazy_segment_tree<Action, Manager>;

public:
    using typename base::action;

    using typename base::operand;
    using typename base::operation;

    using typename base::handler;
    using typename base::node;
    using typename base::pointer;

    using typename base::allocator_type;
    using typename base::size_type;

private:
    size_type len;
    pointer root = handler::nil;

public:
    constexpr ~dynamic_lazy_segment_tree() noexcept {
        base::dispose(root);
    }

    constexpr dynamic_lazy_segment_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : base(alloc) {}

    constexpr dynamic_lazy_segment_tree(const dynamic_lazy_segment_tree &other,
                                        const allocator_type &alloc) noexcept
        : base(alloc), len(other.len), root(other.root) {}

    constexpr dynamic_lazy_segment_tree(dynamic_lazy_segment_tree &&other,
                                        const allocator_type &alloc) noexcept
        : base(alloc), len(other.len), root(other.root) {}

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr dynamic_lazy_segment_tree(
        R &&r, const allocator_type &alloc = allocator_type()) noexcept
        : dynamic_lazy_segment_tree(alloc) {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        base::dispose(root);

        if (len == 0) {
            root = handler::nil;
        } else {
            auto it = std::ranges::begin(r);
            root = base::build(it, 0, len);
        }
    }

    [[nodiscard]] constexpr operand get(size_type k) const noexcept {
        assert(k < len);
        return base::get(root, k, 0, len, operation());
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(k < len);
        base::set(root, k, x, 0, len);
    }

    constexpr void add(size_type k, const operand &x) noexcept {
        assert(k < len);
        base::add(root, k, x, 0, len);
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);
        return base::fold(root, l, r, 0, len, operation());
    }

    [[nodiscard]] constexpr operand fold() const noexcept {
        return root->sum;
    }

    constexpr void copy(const dynamic_lazy_segment_tree &other, size_type l,
                        size_type r) noexcept {
        assert(l < r && r <= len);
        base::copy(root, other.root, l, r, 0, len, operation(), operation());
    }

    constexpr void apply(size_type l, size_type r, const operation &f) noexcept {
        assert(l < r && r <= len);
        base::apply(root, l, r, f, 0, len, operation());
    }
};
} // namespace mld

#endif // MALLARD_DYNAMIC_LAZY_SEGMENT_TREE_H
