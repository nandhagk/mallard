#ifndef MALLARD_ORDERED_SET_H
#define MALLARD_ORDERED_SET_H 1

#include <array>
#include <cassert>
#include <numeric>
#include <optional>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/internal/ordered_set.h"
#include "lib/prelude.h"

namespace mld {

template <typename T, typename Manager, u32 leaf = 16, u32 load = leaf * 3 / 4>
struct ordered_set : private internal::ordered_set<T, Manager, leaf, load> {
private:
    using base = internal::ordered_set<T, Manager, leaf, load>;

public:
    using typename base::value_type;

    using typename base::handler;
    using typename base::node;
    using typename base::pointer;

    using typename base::allocator_type;
    using typename base::size_type;

private:
    pointer root = handler::nil;

public:
    constexpr ~ordered_set() noexcept {
        base::dispose(root);
    }

    constexpr ordered_set(const allocator_type &alloc = allocator_type()) noexcept
        : base(alloc) {}

    template <typename R>
        requires internal::sized_input_range_of<value_type, R>
    explicit constexpr ordered_set(
        R &&r, const allocator_type &alloc = allocator_type()) noexcept
        : base(alloc) {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<value_type, R>
    constexpr void build(R &&r) noexcept {
        base::dispose(root);
        root = base::build(std::forward<R>(r));
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return root->len;
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return root->len == 0;
    }

    constexpr void clear() noexcept {
        base::dispose(root);
        root = handler::nil;
    }

    constexpr bool insert(const value_type &x) noexcept {
        return base::insert(root, x);
    }

    constexpr bool erase(const value_type &x) noexcept {
        return base::erase(root, x);
    }

    [[nodiscard]] constexpr bool contains(const value_type &x) const noexcept {
        return base::contains(root, x);
    }

    [[nodiscard]] constexpr std::optional<value_type>
    prev(const value_type &x) const noexcept {
        return base::prev(root, x);
    }

    [[nodiscard]] constexpr std::optional<value_type>
    next(const value_type &x) const noexcept {
        return base::next(root, x);
    }

    [[nodiscard]] constexpr value_type kth(size_type k) const noexcept {
        assert(k < root->len);
        return base::kth(root, k);
    }

    [[nodiscard]] constexpr size_type rank(const value_type &x) const noexcept {
        return base::rank(root, x);
    }
};
} // namespace mld

#endif // MALLARD_ORDERED_SET_H
