#ifndef MALLARD_OFFLINE_MO_TREE_H
#define MALLARD_OFFLINE_MO_TREE_H 1

#include <cmath>
#include <vector>

#include "lib/mo_order.h"
#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"

namespace mld::offline {
struct mo_tree {
    using size_type = u32;

private:
    size_type len;
    mld::rooted_tree_lca lca;
    std::vector<size_type> tin, tor;
    std::vector<std::pair<size_type, size_type>> que;

public:
    constexpr mo_tree() noexcept = default;

    template <typename Tree>
    explicit constexpr mo_tree(const Tree &g) noexcept {
        build(g);
    }

    template <typename Tree>
    constexpr void build(const Tree &g) noexcept {
        len = static_cast<size_type>(g.size());
        lca.build(g);

        tin.resize(len);
        tor.clear(), tor.reserve(2 * len);

        for (u32 u : lca.tor) {
            tin[u] = static_cast<u32>(tor.size());
            tor.push_back(u);

            for (u32 v = u; v != 0 && lca.tor[lca.tin[v] + lca.siz[v] - 1] == u;
                 v = lca.par[v]) {
                tor.push_back(v);
            }
        }

        que.clear();
    }

    constexpr void reserve(size_type n) noexcept {
        que.reserve(n);
    }

    constexpr void query(size_type u, size_type v) noexcept {
        assert(u < len && v < len);

        u32 l = tin[u] + 1, r = tin[v] + 1;
        if (l > r) std::swap(l, r);

        que.emplace_back(l, r);
    }

    template <bool edge = false, std::invocable<size_type> A,
              std::invocable<size_type> R,
              std::invocable<size_type, size_type, size_type> ASK>
    constexpr void solve(A &&add, R &&rem, ASK &&ask) noexcept {
        std::vector<u32> flp(len);
        auto f = [&](size_type u) {
            if (flp[u] ^= 1)
                add(u);
            else
                rem(u);
        };

        size_type l = 0, r = 0;
        for (size_type k : mo_order<u32>(que, 2 * len - 1)) {
            auto &&[a, b] = que[k];
            while (l > a) f(tor[--l]);
            while (r < b) f(tor[r++]);
            while (l < a) f(tor[l++]);
            while (r > b) f(tor[--r]);

            u32 u = tor[a - 1], v = tor[b - 1];
            if constexpr (edge) {
                ask(k, u, v);
            } else {
                u32 c = lca.lca(u, v);
                add(c), ask(k, u, v), rem(c);
            }
        }
    }
};
} // namespace mld::offline

#endif // MALLARD_OFFLINE_MO_TREE_H
