#ifndef MALLARD_UNION_FIND_ROLLBACK_H
#define MALLARD_UNION_FIND_ROLLBACK_H 1

#include <cassert>
#include <vector>

#include "lib/prelude.h"

namespace mld {

struct union_find_rollback {
    using size_type = u32;

private:
    size_type len, cnt;
    std::vector<i32> par;
    std::vector<std::pair<size_type, i32>> hst;

    [[nodiscard]] constexpr size_type root(size_type u) noexcept {
        for (; par[u] >= 0; u = par[u]);
        return u;
    }

public:
    constexpr union_find_rollback() noexcept = default;

    explicit constexpr union_find_rollback(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = cnt = n;
        par.assign(len, -1);
        hst.clear();
    }

    constexpr void reserve(size_type n) noexcept {
        hst.reserve(n);
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

        hst.emplace_back(v, par[v]);
        par[u] += par[v];
        par[v] = static_cast<i32>(u);
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

    [[nodiscard]] constexpr size_type ccs() const noexcept {
        return cnt;
    }

    constexpr void rollback() noexcept {
        assert(!hst.empty());

        auto [v, d] = hst.back();

        size_type u = par[v];
        par[u] -= par[v] = d;

        ++cnt, hst.pop_back();
    }

    constexpr void rollback(size_type n) noexcept {
        assert(n <= hst.size());
        while (n--) rollback();
    }

    constexpr void rollback_all() noexcept {
        while (!hst.empty()) rollback();
    }
};

} // namespace mld

#endif // MALLARD_UNION_FIND_ROLLBACK_H
