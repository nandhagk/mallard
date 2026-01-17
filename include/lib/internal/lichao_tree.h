#ifndef MALLARD_INTERNAL_LICHAO_TREE_H
#define MALLARD_INTERNAL_LICHAO_TREE_H 1

#include <algorithm>
#include <limits>
#include <memory>
#include <numeric>

#include "lib/prelude.h"

namespace mld::internal {
template <typename T>
struct line {
    using value_type = T;

private:
    value_type m, c;

public:
    constexpr line() noexcept
        : line(0, std::numeric_limits<value_type>::max()) {}

    constexpr line(value_type a, value_type b) noexcept
        : m(a), c(b) {}

    [[nodiscard]] constexpr value_type operator()(auto x) const noexcept {
        return m * x + c;
    }

    [[nodiscard]] constexpr bool operator!=(const line &r) const noexcept {
        return m != r.m || c != r.c;
    }
};

template <std::integral I, typename Line, typename Manager>
struct lichao_tree {
    using index_type = I;

    using line = Line;
    using value_type = line::value_type;

    struct node;

    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using size_type = u32;
    using allocator_type = typename handler::allocator_type;

protected:
    [[no_unique_address]] handler hlr;

public:
    explicit constexpr lichao_tree(
        const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {}

    struct node {
        pointer l = handler::nil, r = handler::nil;
        line x;

        constexpr node() noexcept = default;

        explicit constexpr node(const line &y) noexcept
            : x(y) {}
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    constexpr void add(pointer &v, line &x, index_type l, index_type r, value_type s,
                       value_type e) noexcept {
        if (v == handler::nil) {
            v = hlr.create(x);
            return;
        }

        value_type u = v->x(l), w = v->x(r);
        if (l + 1 == r) {
            if (s < u) v->x = x;
            return;
        } else if (u <= s && w <= e) {
            return;
        } else if (u >= s && w >= e) {
            v->x = x;
            return;
        } else {
            index_type m = std::midpoint(l, r);
            value_type c = v->x(m), q = x(m);

            v = hlr.clone(v);
            if (c > q) {
                std::swap(v->x, x);
                if (s >= u)
                    add(v->l, x, l, m, u, c);
                else
                    add(v->r, x, m, r, c, w);
            } else {
                if (u >= s)
                    add(v->l, x, l, m, s, q);
                else
                    add(v->r, x, m, r, q, e);
            }
        }
    }

    constexpr void add(pointer &v, line &x, index_type a, index_type b, index_type l,
                       index_type r, value_type s, value_type e) noexcept {
        if (r <= a || b <= l) return;
        if (a <= l && r <= b) {
            line y(x);
            return add(v, y, l, r, s, e);
        }

        if (v != handler::nil) {
            value_type u = v->x(l), w = v->x(r);
            if (u <= s && w <= e) return;
            v = hlr.clone(v);
        } else {
            v = hlr.create();
        }

        index_type m = std::midpoint(l, r);
        value_type q = x(m);

        add(v->l, x, a, b, l, m, s, q);
        add(v->r, x, a, b, m, r, q, e);
    }

    [[nodiscard]] constexpr line query(const pointer &v, index_type l, index_type r,
                                       index_type k) const noexcept {
        if (v == handler::nil) return line();
        if (l + 1 == r) return v->x;

        if (index_type m = std::midpoint(l, r); k < m) {
            line y = query(v->l, l, m, k);
            return v->x(k) <= y(k) ? v->x : y;
        } else {
            line y = query(v->r, m, r, k);
            return v->x(k) <= y(k) ? v->x : y;
        }
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_LICHAO_TREE_H
