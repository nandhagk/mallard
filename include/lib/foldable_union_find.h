#ifndef MALLARD_FOLDABLE_UNION_FIND_H
#define MALLARD_FOLDABLE_UNION_FIND_H 1

#include <cassert>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

template <algebra::internal::abelian_monoid Monoid>
struct foldable_union_find {
    using operand = Monoid;
    using size_type = u32;

private:
    size_type len, cnt;
    std::vector<i32> par;
    std::vector<operand> buf;

    [[nodiscard]] constexpr size_type root(size_type u) noexcept {
        if (par[u] < 0) return u;
        return par[u] = root(par[u]);
    }

public:
    constexpr foldable_union_find() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr foldable_union_find(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = cnt = std::ranges::size(r);
        par.assign(len, -1);

        buf.resize(len);
        std::ranges::copy(r, buf.begin());
    }

    [[nodiscard]] constexpr size_type find(size_type u) noexcept {
        assert(u < len);
        return root(u);
    }

    constexpr bool merge(size_type u, size_type v) noexcept {
        assert(u < len && v < len);

        u = root(u), v = root(v);
        if (u == v) return false;
        if (-par[u] < -par[v]) std::swap(u, v);

        par[u] += par[v];
        par[v] = static_cast<i32>(u);
        buf[u] = buf[u] + buf[v];
        --cnt;

        return true;
    }

    [[nodiscard]] constexpr bool is_same(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        return root(u) == root(v);
    }

    [[nodiscard]] constexpr size_type size(size_type u) noexcept {
        assert(u < len);
        return -par[root(u)];
    }

    constexpr void set(size_type u, const operand &x) noexcept {
        assert(u < len);
        u = root(u), buf[u] = x;
    }

    constexpr void add(size_type u, const operand &x) noexcept {
        assert(u < len);
        u = root(u), buf[u] = buf[u] + x;
    }

    [[nodiscard]] constexpr operand fold(size_type u) noexcept {
        assert(u < len);
        return buf[root(u)];
    }

    [[nodiscard]] constexpr size_type ccs() const noexcept {
        return cnt;
    }
};

} // namespace mld

#endif // MALLARD_FOLDABLE_UNION_FIND_H
