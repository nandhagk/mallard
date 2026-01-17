#ifndef MALLARD_DYNAMIC_SEQUENCE_H
#define MALLARD_DYNAMIC_SEQUENCE_H 1

#include <cassert>
#include <memory>

#include "lib/actions/internal/concepts.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"

namespace mld {

template <actions::internal::action Action, template <typename> typename Tree,
          typename Manager>
struct lazy_dynamic_sequence {
    using action = Action;

    using operand = action::operand;
    using operation = action::operation;

private:
    struct node;
    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using tree = Tree<handler>;

public:
    using size_type = tree::size_type;
    using allocator_type = typename handler::allocator_type;

private:
    struct node : tree::node {
        using base = tree::node;

        using base::l;
        using base::r;

        operand val, sum;
        operation lzy;
        size_type len;
        bool rev;

        constexpr node() noexcept = default;

        explicit constexpr node(const operand &x) noexcept
            : base(), val(x), sum(x), len(1), rev(false) {}

        constexpr void pull() noexcept {
            len = l->len + 1 + r->len;
            sum = l->sum + val + r->sum;
        }

        constexpr void flip() noexcept {
            std::swap(l, r);
            sum = +sum;
            rev ^= true;
        }

        constexpr void push() noexcept {
            if (lzy) {
                if (l != handler::nil) l->apply_at(lzy);
                if (r != handler::nil) r->apply_at(lzy);
                lzy = operation();
            }

            if (rev) {
                if (l != handler::nil) l->flip();
                if (r != handler::nil) r->flip();
                rev = false;
            }
        }

        constexpr void apply_at(const operation &f) noexcept {
            lzy = lzy + f;
            val = action::act(f, val);
            sum = action::act(action::pow(f, len), sum);
        }
    };

private:
    [[no_unique_address]] handler hlr;
    pointer root = handler::nil;

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    template <typename F>
    constexpr void walk(pointer v, F &&f) noexcept {
        if (v == handler::nil) return;

        v->push();
        walk(v->l, f);
        f(v->val);
        walk(v->r, f);
    }

public:
    constexpr lazy_dynamic_sequence() noexcept = default;

    constexpr ~lazy_dynamic_sequence() noexcept {
        dispose(root);
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr lazy_dynamic_sequence(
        R &&r, const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        dispose(root);
        root = tree::build(
            r | std::views::transform([this](auto &&x) { return hlr.create(x); }));
    }

    [[nodiscard]] constexpr operand get(size_type k) noexcept {
        assert(k < root->len);
        auto [a, b, c] = tree::split(root, k, k + 1);

        operand x = b->val;
        root = tree::merge(a, b, c);

        return x;
    }

    constexpr void set(size_type k, const operand &x) noexcept {
        assert(k < root->len);

        auto [a, b, c] = tree::split(root, k, k + 1);
        b->val = x;

        root = tree::merge(a, b, c);
    }

    constexpr void add(size_type k, const operand &x) noexcept {
        assert(k < root->len);

        auto [a, b, c] = tree::split(root, k, k + 1);
        b->val = b->val + x;

        root = tree::merge(a, b, c);
    }

    constexpr void insert(size_type k, const operand &x) noexcept {
        assert(k <= root->len);

        auto [a, c] = tree::split(root, k);
        root = tree::merge(tree::merge(a, hlr.create(x)), c);
    }

    constexpr void erase(size_type k) noexcept {
        assert(k < root->len);

        auto [a, b, c] = tree::split(root, k, k + 1);
        root = tree::merge(a, c);

        dispose(b);
    }

    constexpr void reverse(size_type l, size_type r) noexcept
        requires algebra::internal::reversible<operand>
    {
        assert(l < r && r <= root->len);

        auto [a, b, c] = tree::split(root, l, r);
        b->flip();

        root = tree::merge(a, b, c);
    }

    [[nodiscard]] constexpr operand fold(size_type l, size_type r) noexcept {
        assert(l < r && r <= root->len);

        auto [a, b, c] = tree::split(root, l, r);

        operand x = b->sum;
        root = tree::merge(a, b, c);

        return x;
    }

    constexpr void apply(size_type l, size_type r, const operation &f) noexcept {
        assert(l < r && r <= root->len);

        auto [a, b, c] = tree::split(root, l, r);
        b->apply_at(f);

        root = tree::merge(a, b, c);
    }

    template <std::invocable<operand> F>
    constexpr void walk(F &&f) noexcept {
        return walk(root, std::forward<F>(f));
    }
};
} // namespace mld

#endif // MALLARD_DYNAMIC_SEQUENCE_H
