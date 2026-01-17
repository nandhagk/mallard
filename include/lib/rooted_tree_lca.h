#ifndef MALLARD_ROOTED_TREE_LCA_H
#define MALLARD_ROOTED_TREE_LCA_H 1

#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/rooted_tree.h"
#include "lib/sparse_table.h"

namespace mld {
struct rooted_tree_lca : rooted_tree {
    using size_type = rooted_tree::size_type;

    using rooted_tree::is_ancestor;
    using rooted_tree::tin, rooted_tree::dep, rooted_tree::tor, rooted_tree::par;

private:
    using rooted_tree::len;
    four_russian<sparse_table<algebra::min<size_type>>> agg;

public:
    constexpr rooted_tree_lca() noexcept = default;

    template <typename Tree>
    explicit constexpr rooted_tree_lca(const Tree &g) noexcept
        : rooted_tree_lca(g, 0) {}

    template <typename Tree>
    constexpr rooted_tree_lca(const Tree &g, size_type root) noexcept {
        build(g, root);
    }

    template <typename Tree>
    constexpr void build(const Tree &g, size_type root = 0) noexcept {
        rooted_tree::build(g, root);
        if (len == 1) return;

        agg.build(tor | std::views::drop(1) |
                  std::views::transform([&](size_type u) { return tin[par[u]]; }));
    }

    [[nodiscard]] constexpr size_type lca(size_type u, size_type v) const noexcept {
        assert(u < len && v < len);
        if (u == v) return u;

        std::tie(u, v) = std::minmax(tin[u], tin[v]);
        return tor[agg.fold(u, v).val()];
    }

    [[nodiscard]] constexpr size_type dist(size_type u, size_type v) const noexcept {
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }

    [[nodiscard]] constexpr bool is_on_path(size_type u, size_type v,
                                            size_type s) const noexcept {
        return lca(u, v) == s || (is_ancestor(s, u) ^ is_ancestor(s, v));
    }

    [[nodiscard]] constexpr std::optional<std::pair<size_type, size_type>>
    path_intersection(size_type a, size_type b, size_type c, size_type d) {
        size_type ab = lca(a, b), ac = lca(a, c), ad = lca(a, d);
        size_type bc = lca(b, c), bd = lca(b, d), cd = lca(c, d);
        size_type x = ab ^ ac ^ bc, y = ab ^ ad ^ bd;
        if (x != y) return std::pair<size_type, size_type>{x, y};
        size_type z = ac ^ ad ^ cd;
        if (x != z) return std::nullopt;
        return std::pair<size_type, size_type>{x, x};
    }
};
} // namespace mld

#endif // MALLARD_ROOTED_TREE_LCA_H
