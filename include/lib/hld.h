#ifndef MALLARD_HLD_H
#define MALLARD_HLD_H 1

#include <cassert>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

#include "lib/prelude.h"

namespace mld {

struct hld {
    using size_type = u32;

protected:
    size_type len;

public:
    static constexpr size_type UNSET = -1;
    std::vector<size_type> siz, tin, dep, par, tor, bst, beg;

    constexpr hld() noexcept = default;

    template <typename Tree>
    explicit constexpr hld(const Tree &g) noexcept
        : hld(g, 0) {}

    template <typename Tree>
    constexpr hld(const Tree &g, size_type root) noexcept {
        build(g, root);
    }

    template <typename Tree>
    constexpr void build(const Tree &g, size_type root) noexcept {
        len = static_cast<size_type>(g.size());

        siz.assign(len, 1);
        dep.resize(len);
        tin.resize(len);
        par.resize(len);
        bst.assign(len, UNSET);
        beg.resize(len);

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

        for (size_type u : tor | std::views::drop(1) | std::views::reverse) {
            siz[par[u]] += siz[u];
            if (size_type &w = bst[par[u]]; w == UNSET || siz[u] > siz[w]) w = u;
        }

        std::vector<size_type> stk;
        stk.reserve(len), stk.push_back(root);

        tor.clear();
        beg[root] = root;
        while (!stk.empty()) {
            size_type u = stk.back();
            stk.pop_back();

            tin[u] = static_cast<size_type>(tor.size());
            tor.push_back(u);

            for (size_type v : g[u])
                if (v != par[u] && v != bst[u]) beg[v] = v, stk.push_back(v);

            if (bst[u] != UNSET) beg[bst[u]] = beg[u], stk.push_back(bst[u]);
        }
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    [[nodiscard]] constexpr bool is_ancestor(size_type u, size_type v) const noexcept {
        assert(u < len && v < len);
        return tin[u] <= tin[v] && tin[u] + siz[u] > tin[v];
    }

    [[nodiscard]] constexpr std::optional<size_type> jump(size_type u,
                                                          size_type k) const noexcept {
        assert(u < len);
        if (dep[u] < k) return std::nullopt;

        for (; u != UNSET; k -= dep[u] - dep[beg[u]] + 1, u = par[beg[u]])
            if (dep[beg[u]] <= dep[u] - k) return tor[tin[u] - k];

        return u;
    }

    [[nodiscard]] constexpr size_type lca(size_type u, size_type v) const noexcept {
        assert(u < len && v < len);
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;

        for (; beg[u] != beg[v]; v = par[beg[v]])
            if (dep[beg[u]] > dep[beg[v]]) std::swap(u, v);

        return dep[u] < dep[v] ? u : v;
    }

    [[nodiscard]] constexpr size_type dist(size_type u, size_type v) const noexcept {
        assert(u < len && v < len);
        return dep[u] + dep[v] - 2 * dep[lca(u, v)];
    }

    [[nodiscard]] constexpr bool is_on_path(size_type u, size_type v,
                                            size_type s) const noexcept {
        assert(u < len && v < len && s < len);
        return lca(u, v) == s || (is_ancestor(s, u) ^ is_ancestor(s, v));
    }

    [[nodiscard]] constexpr std::optional<size_type> jump(size_type u, size_type v,
                                                          size_type k) const noexcept {
        assert(u < len && v < len);

        if (size_type w = lca(u, v), l = dep[u] - dep[w], r = dep[v] - dep[w];
            l + r < k) {
            return std::nullopt;
        } else if (k < l) {
            return jump(u, k);
        } else {
            return jump(v, l + r - k);
        }
    }

    [[nodiscard]] constexpr std::vector<std::pair<size_type, size_type>>
    decompose(size_type u, size_type v, bool edge = false) const noexcept {
        assert(u < len && v < len);

        std::vector<std::pair<size_type, size_type>> up, dn;
        while (beg[u] != beg[v]) {
            if (tin[u] < tin[v]) {
                dn.emplace_back(beg[v], v);
                v = par[beg[v]];
            } else {
                up.emplace_back(u, beg[u]);
                u = par[beg[u]];
            }
        }

        if (tin[u] < tin[v])
            dn.emplace_back(tor[tin[u] + edge], v);
        else if (tin[v] + edge <= tin[u])
            up.emplace_back(u, tor[tin[v] + edge]);

        up.insert(up.end(), dn.rbegin(), dn.rend());
        return up;
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

#endif // MALLARD_HLD_H
