#ifndef MALLARD_LAZY_LINK_CUT_TREE_H
#define MALLARD_LAZY_LINK_CUT_TREE_H 1

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

#include "lib/actions/internal/concepts.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/internal/link_cut_tree.h"
#include "lib/managers/container.h"

namespace mld {
template <actions::internal::action Action>
struct lazy_link_cut_tree {
    using action = Action;

    using operand = action::operand;
    using operation = action::operation;

    static_assert(algebra::internal::reversible<operand>);

private:
    struct node;

    using container_type = std::vector<node>;

    using handler = mld::managers::container::handler<container_type>;
    using tree = internal::link_cut_tree<handler>;

    using pointer = tree::pointer;

public:
    using size_type = tree::size_type;

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

    [[no_unique_address]] handler hlr;
    size_type len;
    container_type buf;

    [[nodiscard]] constexpr size_type pos(pointer v) noexcept {
        return static_cast<size_type>(v - buf.data());
    }

public:
    constexpr lazy_link_cut_tree() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr lazy_link_cut_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        buf.clear();

        buf.reserve(len);
        for (auto &&a : r) buf.emplace_back(a);
    }

    constexpr void link(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        tree::link(&buf[u], &buf[v]);
    }

    constexpr void cut(size_type u) noexcept {
        assert(u < len);
        tree::cut(&buf[u]);
    }

    constexpr void cut(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        tree::cut(&buf[u], &buf[v]);
    }

    constexpr void reroot(size_type u) noexcept {
        assert(u < len);
        tree::evert(&buf[u]);
    }

    [[nodiscard]] constexpr bool is_connected(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        return tree::is_connected(&buf[u], &buf[v]);
    }

    [[nodiscard]] constexpr size_type lca(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        return pos(tree::lca(&buf[u], &buf[v]));
    }

    [[nodiscard]] constexpr size_type lca(size_type u, size_type v,
                                          size_type r) noexcept {
        reroot(r);
        return lca(u, v);
    }

    [[nodiscard]] std::optional<size_type> jump(size_type u, size_type k) noexcept {
        assert(u < len);
        if (pointer v = tree::jump(&buf[u], k); v != handler::nil)
            return pos(v);
        else
            return std::nullopt;
    }

    [[nodiscard]] std::optional<size_type> jump(size_type u, size_type v,
                                                size_type k) noexcept {
        assert(v < len);
        reroot(v);
        return jump(u, k);
    }

    [[nodiscard]] constexpr size_type root(size_type u) noexcept {
        assert(u < len);
        return pos(tree::root(&buf[u]));
    }

    [[nodiscard]] constexpr size_type parent(size_type u) noexcept {
        assert(u < len);
        return pos(tree::parent(&buf[u]));
    }

    [[nodiscard]] constexpr operand get(size_type u) noexcept {
        assert(u < len);
        tree::expose(&buf[u]);
        return buf[u].val;
    }

    constexpr void set(size_type u, const operand &x) noexcept {
        assert(u < len);
        tree::expose(&buf[u]);
        buf[u].val = x;
        buf[u].pull();
    }

    constexpr void add(size_type u, const operand &x) noexcept {
        assert(u < len);
        tree::expose(&buf[u]);
        buf[u].val = buf[u].val + x;
        buf[u].pull();
    }

    [[nodiscard]] constexpr operand fold_path(size_type v) noexcept {
        assert(v < len);
        tree::expose(&buf[v]);
        return buf[v].sum;
    }

    [[nodiscard]] constexpr operand fold_path(size_type u, size_type v) noexcept {
        reroot(u);
        return fold_path(v);
    }

    constexpr void apply_path(size_type v, const operation &f) noexcept {
        assert(v < len);
        tree::expose(&buf[v]);
        buf[v].apply_at(f);
        buf[v].push();
    }

    constexpr void apply_path(size_type u, size_type v, const operation &f) noexcept {
        reroot(u);
        apply_path(v, f);
    }
};
} // namespace mld

#endif // MALLARD_LINK_CUT_TREE_H
