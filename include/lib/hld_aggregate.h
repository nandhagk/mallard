#ifndef MALLARD_HLD_AGGREGATE_H
#define MALLARD_HLD_AGGREGATE_H 1

#include <ranges>

#include "lib/algebra/internal/concepts.h"
#include "lib/hld.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <typename Aggregate>
struct hld_aggregate : hld {
    using aggregate = Aggregate;
    using operand = aggregate::operand;
    using operation = algebra::internal::operation_type_t<Aggregate>;
    static_assert(algebra::internal::reversible<operand>);

    using size_type = typename hld::size_type;
    using hld::siz, hld::tin, hld::par, hld::tor, hld::bst, hld::beg;

protected:
    using hld::len;
    aggregate agg;

public:
    constexpr hld_aggregate() noexcept = default;

    template <typename Tree, typename R>
    constexpr hld_aggregate(const Tree &g, R &&r) noexcept
        : hld_aggregate(g, 0, std::forward<R>(r)) {}

    template <typename Tree, typename R>
    constexpr hld_aggregate(const Tree &g, size_type root, R &&r) noexcept {
        build(g, root, std::forward<R>(r));
    }

    template <typename Tree, typename R>
        requires internal::sized_range_of<operand, R> &&
                 std::ranges::random_access_range<R> &&
                 algebra::internal::buildable<aggregate, R>
    constexpr void build(const Tree &g, size_type root, R &&r) noexcept {
        hld::build(g, root);
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

    [[nodiscard]] constexpr operand fold_path(size_type u) noexcept {
        assert(u < len);

        operand x;
        for (; u != hld::UNSET; u = par[beg[u]])
            x = agg.fold(tin[beg[u]], tin[u] + 1) + x;

        return +x;
    }

    [[nodiscard]] constexpr operand fold_path(size_type u, size_type v,
                                              bool edge = false) noexcept {
        assert(u < len && v < len);

        operand xl, xr;
        while (beg[u] != beg[v]) {
            if (tin[u] < tin[v]) {
                xr = agg.fold(tin[beg[v]], tin[v] + 1) + xr;
                v = par[beg[v]];
            } else {
                xl = agg.fold(tin[beg[u]], tin[u] + 1) + xl;
                u = par[beg[u]];
            }
        }

        if (tin[u] < tin[v])
            xr = agg.fold(tin[u] + edge, tin[v] + 1) + xr;
        else if (tin[v] + edge <= tin[u])
            xl = agg.fold(tin[v] + edge, tin[u] + 1) + xl;

        return +xl + xr;
    }

    [[nodiscard]] constexpr operand fold_subtree(size_type u) noexcept
        requires algebra::internal::commutative<operand>
    {
        assert(u < len);
        return agg.fold(tin[u], tin[u] + siz[u]);
    }

    [[nodiscard]] constexpr operand fold_outtree(size_type u) noexcept
        requires algebra::internal::commutative<operand>
    {
        assert(u < len);
        return (tin[u] != 0 ? agg.fold(0, tin[u]) : operand()) +
               (tin[u] + siz[u] != len ? agg.fold(tin[u] + siz[u], len) : operand());
    }

    [[nodiscard]] constexpr operand fold() noexcept
        requires algebra::internal::commutative<operand>
    {
        return agg.fold();
    }

    constexpr void apply_path(size_type u, size_type v, const operation &f,
                              bool edge = false) noexcept {
        assert(u < len && v < len);

        while (beg[u] != beg[v]) {
            if (tin[u] < tin[v]) {
                agg.apply(tin[beg[v]], tin[v] + 1, f);
                v = par[beg[v]];
            } else {
                agg.apply(tin[beg[u]], tin[u] + 1, f);
                u = par[beg[u]];
            }
        }

        if (tin[u] < tin[v])
            agg.apply(tin[u] + edge, tin[v] + 1, f);
        else if (tin[v] + edge <= tin[u])
            agg.apply(tin[v] + edge, tin[u] + 1, f);
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

#endif // MALLARD_HLD_AGGREGATE_H
