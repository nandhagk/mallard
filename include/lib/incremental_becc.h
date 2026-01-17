#ifndef MALLARD_INCREMENTAL_BECC_H
#define MALLARD_INCREMENTAL_BECC_H 1

#include <numeric>
#include <unordered_set>
#include <vector>

#include "lib/prelude.h"
#include "lib/union_find.h"

namespace mld {
struct incremental_becc {
    using size_type = u32;

private:
    size_type b;
    union_find cc, becc;
    std::vector<size_type> par;

    [[nodiscard]] constexpr size_type root(size_type u) noexcept {
        return par[u] = becc.find(par[u]);
    }

    constexpr void evert(size_type u) noexcept {
        std::vector<size_type> v;

        for (;;) {
            v.push_back(u);
            if (becc.is_same(u, par[u])) break;
            u = root(u);
        }

        while (!v.empty()) {
            u = v.back();
            v.pop_back();
            par[u] = (v.empty() ? u : v.back());
        }
    }

    constexpr void merge_cc(size_type u, size_type v) noexcept {
        if (cc.size(u) > cc.size(v)) std::swap(u, v);

        evert(u);
        par[u] = v;
        cc.merge(u, v);

        ++b;
    }

    [[nodiscard]] size_type lca(size_type u, size_type v) noexcept {
        constexpr size_type UNSET = -1;

        // TODO: Use something other than unordered_set?
        std::unordered_set<size_type> z;
        for (;;) {
            if (u != UNSET) {
                if (!z.insert(u).second) return u;

                size_type p = root(u);
                u = becc.is_same(u, p) ? UNSET : p;
            }

            std::swap(u, v);
        }
    }

    size_type merge_becc(size_type u, size_type v) noexcept {
        size_type erased_bridge = 0;

        size_type l = lca(u, v);
        while (!becc.is_same(u, l)) {
            size_type p = root(u);
            becc.merge(u, l);
            u = p;
            ++erased_bridge;
        }

        while (!becc.is_same(v, l)) {
            size_type p = root(v);
            becc.merge(v, l);
            v = p;
            ++erased_bridge;
        }

        par[becc.find(l)] = par[l];
        b -= erased_bridge;

        return erased_bridge;
    }

public:
    constexpr incremental_becc() noexcept = default;

    explicit constexpr incremental_becc(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        b = 0;
        cc.build(n);
        becc.build(n);

        par.resize(n);
        std::iota(par.begin(), par.end(), 0);
    }

    bool merge(size_type u, size_type v) noexcept {
        u = becc.find(u), v = becc.find(v);
        if (u == v) return false;

        if (!cc.is_same(u, v)) return merge_cc(u, v), false;
        return merge_becc(u, v), true;
    }

    [[nodiscard]] constexpr size_type size(size_type u) noexcept {
        return becc.size(u);
    }

    [[nodiscard]] constexpr size_type find(size_type u) noexcept {
        return becc.find(u);
    }

    [[nodiscard]] constexpr size_type ccs() const noexcept {
        return cc.ccs() + b;
    }

    [[nodiscard]] constexpr size_type bridges() const noexcept {
        return b;
    }

    [[nodiscard]] constexpr bool is_same(size_type u, size_type v) noexcept {
        return cc.is_same(u, v) && becc.is_same(u, v);
    }
};
} // namespace mld

#endif // MALLARD_INCREMENTAL_BECC_H
