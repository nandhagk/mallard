#ifndef MALLARD_POTENTIALIZED_UNION_FIND_H
#define MALLARD_POTENTIALIZED_UNION_FIND_H 1

#include <cassert>
#include <optional>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

template <algebra::internal::group Group>
struct potentialized_union_find {
    using operand = Group;

    using size_type = u32;

private:
    size_type len, cnt;
    std::vector<i32> par;
    std::vector<operand> buf;

    constexpr size_type root(size_type u) noexcept {
        if (par[u] < 0) return u;
        size_type p = root(par[u]);
        buf[u] = buf[par[u]] + buf[u];
        return par[u] = static_cast<i32>(p);
    }

public:
    constexpr potentialized_union_find() noexcept = default;

    explicit constexpr potentialized_union_find(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = cnt = n;
        par.assign(len, -1);
        buf.assign(len, operand());
    }

    [[nodiscard]] constexpr size_type find(size_type u) noexcept {
        assert(u < len);
        return root(u);
    }

    constexpr bool merge(size_type u, size_type v, operand x) noexcept {
        assert(u < len && v < len);

        size_type a = root(u), b = root(v);
        if (a == b) return false;

        operand y = buf[u], z = buf[v];
        if (-par[a] < -par[b]) {
            std::swap(a, b);
            std::swap(y, z);
            x = -x;
        }

        x = y + -(z + x);

        par[a] += par[b];
        par[b] = static_cast<i32>(a);
        buf[b] = x;
        --cnt;

        return true;
    }

    [[nodiscard]] constexpr bool is_same(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        return root(u) == root(v);
    }

    [[nodiscard]] constexpr operand potential(size_type u) noexcept {
        assert(u < len);
        root(u);
        return buf[u];
    }

    [[nodiscard]] constexpr std::optional<operand> potential(size_type u,
                                                             size_type v) noexcept {
        assert(u < len && v < len);
        if (size_type a = root(u), b = root(v); a != b) return std::nullopt;
        return -buf[v] + buf[u];
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

#endif // MALLARD_POTENTIALIZED_UNION_FIND_H
