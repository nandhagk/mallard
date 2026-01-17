#ifndef MALLARD_INTERNAL_LINK_CUT_TREE_H
#define MALLARD_INTERNAL_LINK_CUT_TREE_H 1

#include <cassert>

#include "lib/splay_tree.h"

namespace mld::internal {
template <typename handler>
struct link_cut_tree {
    using tree = splay_tree<handler>;

    using pointer = tree::pointer;
    using size_type = tree::size_type;

    struct node : tree::node {
        using base = tree::node;

        using base::p;

        [[nodiscard]] constexpr bool is_root() const noexcept {
            return p == handler::nil || (p->l != this && p->r != this);
        }
    };

    static constexpr void splay(pointer v) noexcept {
        for (v->push(); !v->is_root();) {
            if (pointer p = v->p, q = p->p; p->is_root()) {
                if (p->push(), v->push(); p->l == v)
                    tree::zig(p, v);
                else
                    tree::zag(p, v);

                v->p = q;
            } else {
                pointer r = q->p;
                if (q->push(), p->push(), v->push(); q->l == p)
                    if (p->l == v)
                        tree::zig_zig(q, p, v);
                    else
                        tree::zig_zag(q, p, v);
                else //
                    if (p->r == v)
                        tree::zag_zag(q, p, v);
                    else
                        tree::zag_zig(q, p, v);

                if ((v->p = r)) {
                    if (r->l == q) r->l = v;
                    if (r->r == q) r->r = v;
                }
            }
        }
    }

    static constexpr pointer expose(pointer v) noexcept {
        pointer c = handler::nil;
        for (pointer u = v; u != handler::nil; c = u, u = u->p) {
            splay(u);
            u->r = c;
            u->pull();
        }
        splay(v);
        return c;
    }

    static constexpr void evert(pointer v) noexcept {
        expose(v);
        v->flip();
        v->push();
    }

    static constexpr void link(pointer p, pointer c) noexcept {
        evert(c);
        expose(p);
        c->p = p;
        p->r = c;
        p->pull();
    }

    static constexpr void cut(pointer c) noexcept {
        expose(c);
        pointer p = c->l;
        if (p == handler::nil) return;
        c->l = p->p = handler::nil;
        c->pull();
    }

    static constexpr void cut(pointer u, pointer v) noexcept {
        evert(u);
        cut(v);
    }

    [[nodiscard]] static constexpr pointer lca(pointer u, pointer v) noexcept {
        return expose(u), expose(v);
    }

    [[nodiscard]] static constexpr bool is_connected(pointer u, pointer v) noexcept {
        expose(u), expose(v);
        return u == v || u->p != handler::nil;
    }

    [[nodiscard]] static constexpr pointer jump(pointer u, size_type k) noexcept {
        for (expose(u); u != handler::nil;) {
            u->push();
            if (size_type len = u->r->len; k == len) {
                return u;
            } else if (k < len) {
                u = u->r;
            } else {
                k -= len + 1;
                u = u->l;
            }
        }

        return handler::nil;
    }

    [[nodiscard]] static constexpr pointer root(pointer u) noexcept {
        for (expose(u); u->l != handler::nil; u = u->l) u->push();
        return u;
    }

    [[nodiscard]] static constexpr pointer parent(pointer x) noexcept {
        expose(x);

        pointer c = handler::nil;
        for (pointer p = x->l; p != handler::nil; c = p, p = p->r) p->push();

        return c;
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_LINK_CUT_TREE_H
