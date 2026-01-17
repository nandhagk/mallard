#ifndef MALLARD_BASIC_DYNAMIC_SEQUENCE_H
#define MALLARD_BASIC_DYNAMIC_SEQUENCE_H 1

#include <cassert>
#include <memory>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"

namespace mld {

template <typename T, template <typename> typename Tree, typename Manager>
struct basic_dynamic_sequence {
    using value_type = T;

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

        value_type val;
        size_type len;
        bool rev;

        constexpr node() noexcept = default;

        explicit constexpr node(const value_type &x) noexcept
            : base(), val(x), len(1), rev(false) {}

        constexpr void pull() noexcept {
            len = l->len + 1 + r->len;
        }

        constexpr void flip() noexcept {
            std::swap(l, r);
            rev ^= true;
        }

        constexpr void push() noexcept {
            if (rev) {
                if (l != handler::nil) l->flip();
                if (r != handler::nil) r->flip();
                rev = false;
            }
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
    constexpr basic_dynamic_sequence() noexcept = default;

    constexpr ~basic_dynamic_sequence() noexcept {
        dispose(root);
    }

    template <typename R>
        requires internal::sized_input_range_of<value_type, R>
    explicit constexpr basic_dynamic_sequence(
        R &&r, const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<value_type, R>
    constexpr void build(R &&r) noexcept {
        dispose(root);
        root = tree::build(
            r | std::views::transform([this](auto &&x) { return hlr.create(x); }));
    }

    [[nodiscard]] constexpr value_type get(size_type k) noexcept {
        assert(k < root->len);
        auto [a, b, c] = tree::split(root, k, k + 1);

        value_type x = b->val;
        root = tree::merge(a, b, c);

        return x;
    }

    constexpr void insert(size_type k, const value_type &x) noexcept {
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

    constexpr void reverse(size_type l, size_type r) noexcept {
        assert(l < r && r <= root->len);

        auto [a, b, c] = tree::split(root, l, r);
        b->flip();

        root = tree::merge(a, b, c);
    }

    template <std::invocable<value_type> F>
    constexpr void walk(F &&f) noexcept {
        return walk(root, std::forward<F>(f));
    }
};
} // namespace mld

#endif // MALLARD_BASIC_DYNAMIC_SEQUENCE_H
