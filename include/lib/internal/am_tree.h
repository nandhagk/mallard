#ifndef MALLARD_INTERNAL_AM_TREE_H
#define MALLARD_INTERNAL_AM_TREE_H 1

#include <cassert>
#include <optional>
#include <utility>
#include <vector>

#include "lib/prelude.h"

namespace mld::internal {

template <typename T, typename handler>
struct am_tree {
    using size_type = u32;
    using cost_type = T;

    using pointer = handler::pointer;

    struct node {
        pointer p = handler::nil;
        size_type len;
        cost_type cst;

        constexpr node() noexcept = default;

        explicit constexpr node(bool) noexcept
            : len(1) {}
    };

    using result = std::optional<std::optional<cost_type>>;

    [[nodiscard]] static constexpr result insert(pointer u, pointer v,
                                                 cost_type w) noexcept {
        if (u == v) return std::nullopt;
        return balance(u), balance(v), link(u, v, w);
    }

    [[nodiscard]] static constexpr bool is_connected(pointer u, pointer v) noexcept {
        if (u == v) return true;
        for (balance(u), balance(v);;) {
            if (u->len > v->len) std::swap(u, v);
            if (u = u->p; u == handler::nil) return false;
            if (u == v) return true;
        }
    }

    [[nodiscard]] static constexpr std::optional<cost_type>
    max_path(pointer u, pointer v) noexcept {
        if (u == v) return std::nullopt;

        balance(u), balance(v);
        for (std::optional<cost_type> res = std::nullopt;;) {
            if (u->len > v->len) std::swap(u, v);
            if (!res || u->cst > *res) res = u->cst;

            if (u = u->p; u == handler::nil) return std::nullopt;
            if (u == v) return res;
        }
    }

    [[nodiscard]] static constexpr result erase(pointer u, pointer v,
                                                cost_type w) noexcept {
        if (u == v) return std::nullopt;
        return balance(u), balance(v), cut_max_path(u, v, w);
    }

    [[nodiscard]] static constexpr pointer root(pointer u) noexcept {
        for (balance(u); u->p != handler::nil; u = u->p);
        return u;
    }

private:
    static constexpr void promote(pointer u) noexcept {
        if (pointer p = u->p; u->cst >= p->cst && p->p != handler::nil) {
            u->p = p->p;
            p->len -= u->len;
        } else {
            u->p = std::exchange(p->p, u);
            std::swap(u->cst, p->cst);
            p->len -= u->len;
            u->len += p->len;
        }
    }

    [[nodiscard]] static constexpr result cut_max_path(pointer u, pointer v,
                                                       cost_type w) noexcept {
        assert(u != v);

        for (pointer t = handler::nil;;) {
            if (u->len > v->len) std::swap(u, v);

            pointer p = u->p;
            if (p == handler::nil) return std::optional<cost_type>(std::nullopt);

            if (t == handler::nil || u->cst > t->cst) t = u;
            if (u = p; u == v) {
                if (w >= t->cst) return std::nullopt;

                for (pointer s = t->p; s != handler::nil; s->len -= t->len, s = s->p);
                t->p = handler::nil;

                return std::optional<cost_type>(t->cst);
            }
        }
    }

    [[nodiscard]] static constexpr result link(pointer u, pointer v,
                                               cost_type w) noexcept {
        assert(u != v);

        result res = cut_max_path(u, v, w);
        if (!res) return std::nullopt;

        for (size_type a = 0, b = 0;;) {
            for (; u->p != handler::nil && w >= u->cst; u = u->p, u->len += a);
            for (; v->p != handler::nil && w >= v->cst; v = v->p, v->len += b);
            if (u->len > v->len) {
                std::swap(u, v);
                std::swap(a, b);
            }

            a -= u->len;
            b += u->len;
            v->len += u->len;
            std::swap(u->cst, w);

            if (u = std::exchange(u->p, v); u == handler::nil) {
                for (v = v->p; v != handler::nil; v->len += b, v = v->p);
                return res;
            }

            u->len += a;
        }
    }

    static constexpr void balance(pointer u) noexcept {
        for (pointer p = u->p; p != handler::nil; p = u->p)
            if (u->len * 3 / 2 > p->len)
                promote(u);
            else
                u = p;
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_AM_TREE_H
