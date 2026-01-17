#ifndef MALLARD_BASIC_LINK_CUT_TREE_H
#define MALLARD_BASIC_LINK_CUT_TREE_H 1

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/internal/link_cut_tree.h"
#include "lib/managers/container.h"

namespace mld {
struct basic_link_cut_tree {
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

        size_type len;
        bool rev;

        constexpr node() noexcept = default;

        // To differentiate from default
        explicit constexpr node(bool) noexcept
            : base(), len(1), rev(false) {}

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

    [[no_unique_address]] handler hlr;
    size_type len;
    container_type buf;

    [[nodiscard]] constexpr size_type pos(pointer v) noexcept {
        return static_cast<size_type>(v - buf.data());
    }

public:
    constexpr basic_link_cut_tree() noexcept = default;

    explicit constexpr basic_link_cut_tree(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        buf.assign(len, node(true));
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
};
} // namespace mld

#endif // MALLARD_BASIC_LINK_CUT_TREE_H
