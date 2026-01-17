#ifndef MALLARD_SPLAY_TREE_H
#define MALLARD_SPLAY_TREE_H 1

#include <concepts>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>

#include "lib/algebra/internal/concepts.h"
#include "lib/algebra/sum.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <typename handler>
struct splay_tree {
    using size_type = u32;
    using pointer = handler::pointer;

    struct node {
        pointer l = handler::nil, r = handler::nil, p = handler::nil;
    };

    static constexpr void setl(pointer p, pointer c) noexcept {
        p->push();
        if ((p->l = c) != handler::nil) c->p = p;
        p->pull();
    }

    static constexpr void setr(pointer p, pointer c) noexcept {
        p->push();
        if ((p->r = c) != handler::nil) c->p = p;
        p->pull();
    }

    static constexpr void cutl(pointer p) noexcept {
        p->push();
        if (p->l != handler::nil) {
            p->l->p = handler::nil;
            p->l = handler::nil;
            p->pull();
        }
    }

    static constexpr void cutr(pointer p) noexcept {
        p->push();
        if (p->r != handler::nil) {
            p->r->p = handler::nil;
            p->r = handler::nil;
            p->pull();
        }
    }

    [[nodiscard]] static constexpr pointer root(pointer v) noexcept {
        for (; v->p != handler::nil; v = v->p);
        return v;
    }

    [[nodiscard]] static constexpr pointer leftmost(pointer v) noexcept {
        for (; v->l != handler::nil; v = v->l) v->push();
        return v;
    }

    [[nodiscard]] static constexpr pointer rightmost(pointer v) noexcept {
        for (; v->r != handler::nil; v = v->r) v->push();
        return v;
    }

    static constexpr void zig(pointer p, pointer v) noexcept {
        if ((p->l = v->r) != handler::nil) p->l->p = p;
        p->pull();
        v->r = p;
        p->p = v;
        v->pull();
    }

    static constexpr void zag(pointer p, pointer v) noexcept {
        if ((p->r = v->l) != handler::nil) p->r->p = p;
        p->pull();
        v->l = p;
        p->p = v;
        v->pull();
    }

    static constexpr void zig_zig(pointer q, pointer p, pointer v) noexcept {
        if ((q->l = p->r) != handler::nil) q->l->p = q;
        q->pull();
        p->r = q;
        q->p = p;
        zig(p, v);
    }

    static constexpr void zig_zag(pointer q, pointer p, pointer v) noexcept {
        if ((p->r = v->l) != handler::nil) p->r->p = p;
        p->pull();
        if ((q->l = v->r) != handler::nil) q->l->p = q;
        q->pull();
        v->l = p;
        v->r = q;
        p->p = q->p = v;
        v->pull();
    }

    static constexpr void zag_zag(pointer q, pointer p, pointer v) noexcept {
        if ((q->r = p->l) != handler::nil) q->r->p = q;
        q->pull();
        p->l = q;
        q->p = p;
        zag(p, v);
    }

    static constexpr void zag_zig(pointer q, pointer p, pointer v) noexcept {
        if ((p->l = v->r) != handler::nil) p->l->p = p;
        p->pull();
        if ((q->r = v->l) != handler::nil) q->r->p = q;
        q->pull();
        v->r = p;
        v->l = q;
        p->p = q->p = v;
        v->pull();
    }

    static constexpr void splay(pointer v) noexcept {
        for (pointer p = v->p, u = v; p != handler::nil;) {
            if (p->p == handler::nil) {
                if (p->push(), v->push(); p->l == u)
                    zig(p, v);
                else
                    zag(p, v);

                break;
            } else {
                pointer q = p->p, r = q->p;
                if (q->push(), p->push(), v->push(); q->l == p)
                    if (p->l == u)
                        zig_zig(q, p, v);
                    else
                        zig_zag(q, p, v);
                else //
                    if (p->r == u)
                        zag_zag(q, p, v);
                    else
                        zag_zig(q, p, v);

                p = r, u = q;
            }
        }

        v->p = handler::nil;
    }

    template <algebra::internal::monoid Monoid, auto val, auto sum, typename F>
    [[nodiscard]] static constexpr pointer max_right(pointer v, F &&f) noexcept {
        using operand = Monoid;

        if (v == handler::nil || f(sum(v))) return handler::nil;
        for (operand x;;) {
            v->push();

            if (operand y = x + sum(v->l); v->l != handler::nil && !f(y)) {
                v = v->l;
            } else {
                x = y + val(v);
                if (!f(x)) {
                    splay(v);
                    return v;
                }

                v = v->r;
            }
        }
    }

    [[nodiscard]] static std::pair<pointer, pointer> split(pointer v,
                                                           size_type r) noexcept {
        static constexpr auto val = [](pointer) { return 1; };
        static constexpr auto sum = [](pointer u) { return u->len; };

        pointer u = max_right<algebra::sum<size_type>, val, sum>(
            v, [r](auto &&x) { return x <= r; });
        if (u == handler::nil) return {v, handler::nil};

        pointer c = u->l;
        cutl(u);
        return {c, u};
    }

    [[nodiscard]] static constexpr std::tuple<pointer, pointer, pointer>
    split(pointer v, size_type l, size_type r) noexcept {
        pointer a = handler::nil, b = handler::nil, c = handler::nil;

        std::tie(b, c) = split(v, r);
        if (l != 0) {
            std::tie(a, b) = split(b, l - 1);
            pointer B = b->r;
            cutr(b);
            setl(b, a);
            a = b;
            b = B;
        }

        return {a, b, c};
    }

    [[nodiscard]] static constexpr pointer merge(pointer u, pointer v) noexcept {
        if (u == handler::nil) return v;
        if (v == handler::nil) return u;

        if (u->len < v->len) {
            u = rightmost(u);
            splay(u);
            setr(u, v);
            return u;
        } else {
            v = leftmost(v);
            splay(v);
            setl(v, u);
            return v;
        }
    }

    [[nodiscard]] static constexpr pointer merge(pointer a, pointer b,
                                                 pointer c) noexcept {
        // assert(!a || !a->r);
        // assert(!c || !c->l);

        if (a != handler::nil)
            setr(a, b);
        else
            a = b;

        if (c != handler::nil)
            setl(c, a);
        else
            c = a;

        return c;
    }

private:
    template <std::input_iterator InputIt>
    [[nodiscard]] static constexpr pointer build(InputIt &it, size_type l,
                                                 size_type r) noexcept {
        size_type m = std::midpoint(l, r);

        pointer u = handler::nil, w = handler::nil;
        if (l < m) u = build(it, l, m);
        pointer v = *(it++);
        if (m + 1 < r) w = build(it, m + 1, r);

        v->l = u, u->p = v;
        v->r = w, w->p = v;
        v->pull();
        return v;
    }

public:
    template <typename R>
        requires internal::sized_input_range_of<pointer, R>
    [[nodiscard]] static constexpr pointer build(R &&r) noexcept {
        size_type len = static_cast<size_type>(std::ranges::size(r));
        if (len == 0) return handler::nil;

        auto it = std::ranges::begin(r);
        return build(it, 0, len);
    }
};
} // namespace mld

#endif // MALLARD_SPLAY_TREE_H
