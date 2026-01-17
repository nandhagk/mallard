#ifndef MALLARD_ROOTED_TREE_H
#define MALLARD_ROOTED_TREE_H 1

#include <cassert>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

#include "lib/prelude.h"

namespace mld {

struct rooted_tree {
    using size_type = u32;

protected:
    size_type len;

public:
    static constexpr size_type UNSET = -1;
    std::vector<size_type> siz, tin, dep, par, tor;

    constexpr rooted_tree() noexcept = default;

    template <typename Tree>
    explicit constexpr rooted_tree(const Tree &g) noexcept
        : rooted_tree(g, 0) {}

    template <typename Tree>
    constexpr rooted_tree(const Tree &g, size_type root) noexcept {
        build(g, root);
    }

    template <typename Tree>
    constexpr void build(const Tree &g, size_type root = 0) noexcept {
        len = static_cast<size_type>(g.size());

        siz.assign(len, 1);
        dep.resize(len);
        par.resize(len);

        par[root] = UNSET;
        dep[root] = 0;

        tor.clear(), tor.reserve(len);
        tor.push_back(root);

        for (size_type i = 0; i < len; ++i) {
            size_type u = tor[i];
            for (size_type v : g[u]) {
                if (v == par[u]) continue;
                par[v] = u;
                dep[v] = dep[u] + 1;
                tor.push_back(v);
            }
        }

        for (size_type u : tor | std::views::drop(1) | std::views::reverse)
            siz[par[u]] += siz[u];

        tin = siz;
        for (size_type u : tor | std::views::drop(1))
            tin[u] = std::exchange(tin[par[u]], tin[par[u]] - tin[u]);

        for (size_type u = 0; u < len; ++u) tor[--tin[u]] = u;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    [[nodiscard]] constexpr bool is_ancestor(size_type u, size_type v) const noexcept {
        assert(u < len && v < len);
        return tin[u] <= tin[v] && tin[u] + siz[u] > tin[v];
    }
};
} // namespace mld

#endif // MALLARD_ROOTED_TREE_H
