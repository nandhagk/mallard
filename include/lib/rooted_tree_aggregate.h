#ifndef MALLARD_ROOTED_TREE_AGGREGATE_H
#define MALLARD_ROOTED_TREE_AGGREGATE_H 1

#include <ranges>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"

namespace mld {
template <typename Aggregate>
struct rooted_tree_aggregate : rooted_tree {
    using aggregate = Aggregate;
    using operand = aggregate::operand;
    using operation = algebra::internal::operation_type_t<Aggregate>;
    static_assert(algebra::internal::commutative<operand>);

    using size_type = typename rooted_tree::size_type;
    using rooted_tree::siz, rooted_tree::tin, rooted_tree::tor;

private:
    using rooted_tree::len;
    aggregate agg;

public:
    constexpr rooted_tree_aggregate() noexcept = default;

    template <typename Tree, typename R>
    constexpr rooted_tree_aggregate(const Tree &g, R &&r) noexcept
        : rooted_tree_aggregate(g, 0, std::forward<R>(r)) {}

    template <typename Tree, typename R>
    constexpr rooted_tree_aggregate(const Tree &g, size_type root, R &&r) noexcept {
        build(g, root, std::forward<R>(r));
    }

    template <typename Tree, typename R>
        requires internal::sized_range_of<operand, R> &&
                 std::ranges::random_access_range<R> &&
                 algebra::internal::buildable<aggregate, R>
    constexpr void build(const Tree &g, size_type root, R &&r) noexcept {
        rooted_tree::build(g, root);
        agg.build(tor | std::views::transform([&](size_type k) { return r[k]; }));
    }

    [[nodiscard]] constexpr operand get(size_type u) noexcept {
        assert(u < len);
        return agg.get(tin[u]);
    }

    constexpr void set(size_type u, const operand &x) noexcept {
        assert(u < len);
        agg.set(tin[u], x);
    }

    constexpr void add(size_type u, const operand &x) noexcept {
        assert(u < len);
        agg.add(tin[u], x);
    }

    [[nodiscard]] constexpr operand fold_subtree(size_type u) noexcept {
        assert(u < len);
        return agg.fold(tin[u], tin[u] + siz[u]);
    }

    [[nodiscard]] constexpr operand fold_outtree(size_type u) noexcept {
        assert(u < len);
        return (tin[u] != 0 ? agg.fold(0, tin[u]) : operand()) +
               (tin[u] + siz[u] != len ? agg.fold(tin[u] + siz[u], len) : operand());
    }

    [[nodiscard]] constexpr operand fold() noexcept {
        return agg.fold();
    }

    constexpr void apply_subtree(size_type u, const operation &f) noexcept {
        assert(u < len);
        agg.apply(tin[u], tin[u] + siz[u], f);
    }

    constexpr void apply_outtree(size_type u, const operation &f) noexcept {
        assert(u < len);
        if (tin[u] != 0) agg.apply(0, tin[u], f);
        if (tin[u] + siz[u] != len) agg.apply(tin[u] + siz[u], len, f);
    }
};
} // namespace mld

#endif // MALLARD_ROOTED_TREE_AGGREGATE_H
