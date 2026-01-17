#ifndef MALLARD_UNION_FIND_H
#define MALLARD_UNION_FIND_H 1

#include <cassert>
#include <vector>

#include "lib/prelude.h"

namespace mld {

struct union_find {
    using size_type = u32;

private:
    size_type len, cnt;
    std::vector<i32> par;

    [[nodiscard]] constexpr size_type root(size_type u) noexcept {
        if (par[u] < 0) return u;
        return par[u] = root(par[u]);
    }

public:
    constexpr union_find() noexcept = default;

    explicit constexpr union_find(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = cnt = n;
        par.assign(len, -1);
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
};

} // namespace mld

#endif // MALLARD_UNION_FIND_H
