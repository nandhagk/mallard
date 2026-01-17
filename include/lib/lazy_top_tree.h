#ifndef MALLARD_LAZY_TOP_TREE_H
#define MALLARD_LAZY_TOP_TREE_H 1

#include <algorithm>
#include <array>

#include "lib/actions/internal/concepts.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/managers/container.h"
#include "lib/prelude.h"

namespace mld {
template <actions::internal::action Action>
struct lazy_top_tree {
    using action = Action;

    using operand = action::operand;
    using operation = action::operation;

    static_assert(algebra::internal::reversible<operand>);

    using size_type = u32;

protected:
    struct node;

    using container_type = std::vector<node>;

    using handler = mld::managers::container::handler<container_type>;
    using pointer = typename handler::pointer;

    [[no_unique_address]] handler hlr;
    size_type len;
    container_type buf;
    std::vector<pointer> fre;

    struct node {
        pointer l = handler::nil, r = handler::nil, h = handler::nil, p = handler::nil;
        operand val, sum, sub;
        operation lzy, hzy;
        size_type len, cap;
        bool rev = false;

        constexpr node() noexcept = default;

        explicit constexpr node(const operand &x) noexcept
            : val(x), sum(x), len(1) {}

        [[nodiscard]] constexpr bool is_root() const noexcept {
            return p == handler::nil || (p->l != this && p->r != this);
        }

        constexpr void flip() noexcept {
            std::swap(l, r);
            sum = +sum;
            rev ^= true;
        }

        template <bool f>
        constexpr void pull() noexcept {
            if (this == handler::nil) return;

            if constexpr (!f) {
                len = l->len + 1 + r->len;
                sum = l->sum + val + r->sum;
                cap = l->cap + r->cap + h->cap;
                sub = l->sub + r->sub + h->sub;
            } else {
                cap = l->cap + r->cap + h->cap + h->len;
                sub = l->sub + r->sub + h->sub + h->sum;
            }
        }

        constexpr void apply_at(const operation &f) noexcept {
            lzy = lzy + f;
            val = action::act(f, val);
            sum = action::act(action::pow(f, len), sum);
        }

        constexpr void apply_sub(const operation &f) noexcept {
            hzy = hzy + f;
            sub = action::act(action::pow(f, cap), sub);
        }

        template <bool f>
        constexpr void push() noexcept {
            if (this == handler::nil) return;

            if constexpr (!f) {
                if (rev) {
                    if (l != handler::nil) l->flip();
                    if (r != handler::nil) r->flip();
                    rev = false;
                }

                if (lzy) {
                    if (l != handler::nil) l->apply_at(lzy);
                    if (r != handler::nil) r->apply_at(lzy);
                    lzy = operation();
                }

                if (hzy) {
                    if (l != handler::nil) l->apply_sub(hzy);
                    if (r != handler::nil) r->apply_sub(hzy);
                    if (h != handler::nil) h->apply_sub(hzy);
                    hzy = operation();
                }
            } else //
                if (hzy) {
                    if (l != handler::nil) l->apply_sub(hzy);
                    if (r != handler::nil) r->apply_sub(hzy);
                    if (h != handler::nil) {
                        h->apply_sub(hzy);
                        h->apply_at(hzy);
                    }
                    hzy = operation();
                }
        }
    };

    [[nodiscard]] constexpr pointer create() noexcept {
        pointer v;
        if (fre.empty()) {
            buf.emplace_back();
            v = &buf.back();
        } else {
            v = fre.back();
            fre.pop_back();
        }

        v->len = 1;
        return v;
    }

    constexpr void destroy(pointer x) noexcept {
        fre.push_back(x);
        *x = node();
    }

    static constexpr void setl(pointer x, pointer y) noexcept {
        if ((x->l = y) != handler::nil) y->p = x;
    }

    static constexpr void setr(pointer x, pointer y) noexcept {
        if ((x->r = y) != handler::nil) y->p = x;
    }

    static constexpr void seth(pointer x, pointer y) noexcept {
        if ((x->h = y) != handler::nil) y->p = x;
    }

    template <bool f>
    static constexpr void rotate(pointer x) noexcept {
        pointer p = x->p, g = p->p;
        if (g != handler::nil) {
            if (g->h == p)
                g->h = x;
            else if (g->r == p)
                g->r = x;
            else
                g->l = x;
        }

        if (p->r == x) {
            p->r = x->l;
            p->r->p = p;
            x->l = p;
        } else {
            p->l = x->r;
            p->l->p = p;
            x->r = p;
        }

        p->p = x;
        x->p = g;

        p->template pull<f>(), x->template pull<f>();
    }

    template <bool f>
    static constexpr void down(pointer x) noexcept {
        if (!x->is_root()) down<f>(x->p);
        x->template push<f>();
    }

    template <bool f>
    static constexpr void splay(pointer x) noexcept {
        down<f>(x);
        while (!x->is_root()) {
            pointer p = x->p, q = p->p;
            if (!p->is_root()) rotate<f>(((p == q->l) ^ (x == p->l)) ? x : p);
            rotate<f>(x);
        }
    }

    template <bool f>
    static constexpr void down(pointer x, pointer g) noexcept {
        if (!x->is_root() && x->p != g) down<f>(x->p, g);
        x->template push<f>();
    }

    template <bool f>
    static constexpr void splay(pointer x, pointer g) noexcept {
        down<f>(x, g);
        while (!x->is_root() && x->p != g) {
            pointer p = x->p, q = p->p;
            if (!p->is_root() && q != g) rotate<f>(((p == q->l) ^ (x == p->l)) ? x : p);
            rotate<f>(x);
        }
    }

    static constexpr void splay_rightmost(pointer x) noexcept {
        pointer g = x->p;
        for (; x->r != handler::nil; x = x->r) x->template push<1>();
        splay<1>(x, g);
    }

    constexpr void splice(pointer x) noexcept {
        splay<1>(x), x = x->p, splay<0>(x);
        pointer y = x->h;
        y->template push<1>();
        if (x->r != handler::nil) {
            std::swap(x->r->p, y->h->p);
            std::swap(x->r, y->h);
        } else {
            setr(x, y->h);
            if (y->l != handler::nil) {
                splay_rightmost(y->l);
                setr(y->l, y->r);
                x->h = y->l;
            } else {
                y->r->template push<1>();
                x->h = y->r;
            }

            destroy(y);
            if ((y = x->h) != handler::nil) y->p = x;
        }

        y->template pull<1>(), x->template pull<0>();
        x->r->template push<0>();
        rotate<0>(x->r);
    }

    constexpr void expose(pointer x) noexcept {
        splay<0>(x);
        if (x->r != handler::nil) {
            pointer y = create();
            setl(y, x->h);
            seth(y, x->r);
            y->template pull<1>();
            x->r = handler::nil;
            seth(x, y);
            x->template pull<0>();
        }

        while (x->p != handler::nil) splice(x->p);
    }

    constexpr void evert(pointer x) noexcept {
        expose(x);
        x->flip();
    }

    [[nodiscard]] constexpr pointer root(pointer x) noexcept {
        expose(x);
        for (; x->l != handler::nil; x = x->l);
        return splay<0>(x), x;
    }

    constexpr void link(pointer x, pointer y) noexcept {
        evert(y), expose(x);
        setr(x, y);
        x->template pull<0>();
    }

    constexpr void cut(pointer x) noexcept {
        expose(x);
        x->template push<0>();
        if (x->l != handler::nil) {
            x->l->p = handler::nil;
            x->l = handler::nil;
            x->template pull<0>();
        }
    }

    constexpr void apply_subtree(pointer x, const operation &f) noexcept {
        expose(x);
        x->val = action::act(f, x->val);
        x->template pull<0>();
        x->h->apply_sub(f);
    }

    constexpr void apply_path(pointer y, const operation &f) noexcept {
        expose(y);
        y->apply_at(f);
    }

    [[nodiscard]] constexpr operand fold_subtree(pointer x) noexcept {
        expose(x);
        return x->val + x->h->sub;
    }

    [[nodiscard]] constexpr operand fold_path(pointer y) noexcept {
        return expose(y), y->sum;
    }

    [[nodiscard]] constexpr size_type dist(pointer v) noexcept {
        return static_cast<size_type>(v - buf.data());
    }

public:
    constexpr lazy_top_tree() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr lazy_top_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        buf.reserve(len * 2);
        fre.reserve(len);

        for (auto &&a : r) buf.emplace_back(a);
    }

    constexpr void link(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        link(&buf[u], &buf[v]);
    }

    constexpr void reroot(size_type u) noexcept {
        assert(u < len);
        evert(&buf[u]);
    }

    constexpr void cut(size_type u) noexcept {
        assert(u < len);
        cut(&buf[u]);
    }

    constexpr void cut(size_type u, size_type v) noexcept {
        reroot(v);
        cut(u);
    }

    constexpr void apply_subtree(size_type u, const operation &f) noexcept {
        assert(u < len);
        apply_subtree(&buf[u], f);
    }

    constexpr void apply_subtree(size_type r, size_type u,
                                 const operation &f) noexcept {
        reroot(r);
        apply_subtree(u, f);
    }

    constexpr void apply_path(size_type u, const operation &f) noexcept {
        assert(u < len);
        apply_path(&buf[u], f);
    }

    constexpr void apply_path(size_type u, size_type v, const operation &f) noexcept {
        reroot(u);
        apply_path(v, f);
    }

    [[nodiscard]] constexpr operand fold_subtree(size_type u) noexcept {
        assert(u < len);
        return fold_subtree(&buf[u]);
    }

    [[nodiscard]] constexpr operand fold_subtree(size_type r, size_type u) noexcept {
        reroot(r);
        return fold_subtree(u);
    }

    [[nodiscard]] constexpr operand fold_path(size_type u) noexcept {
        assert(u < len);
        return fold_path(&buf[u]);
    }

    [[nodiscard]] constexpr operand fold_path(size_type u, size_type v) noexcept {
        reroot(u);
        return fold_path(v);
    }

    constexpr void set(size_type u, const operand &x) noexcept {
        assert(u < len);
        evert(&buf[u]), expose(&buf[u]);
        buf[u].val = x;
        buf[u].template pull<0>();
    }

    [[nodiscard]] constexpr operand get(size_type u) noexcept {
        assert(u < len);
        evert(&buf[u]), expose(&buf[u]);
        return buf[u].val;
    }
};
} // namespace mld

#endif // MALLARD_LAZY_TOP_TREE_H
