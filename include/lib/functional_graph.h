#include <concepts>
#ifndef MALLARD_FUNCTIONAL_GRAPH_H
#define MALLARD_FUNCTIONAL_GRAPH_H 1

#include <cassert>
#include <vector>

#include "lib/hld.h"
#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

namespace mld {
struct functional_graph {
    using size_type = u32;

private:
    size_type len;
    std::vector<size_type> nxt, rot;
    hld h;

public:
    constexpr functional_graph() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    explicit constexpr functional_graph(R &&r) {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));

        nxt.resize(len);
        std::ranges::copy(r, nxt.begin());

        rot.assign(len, 0);

        mld::union_find uf(len);
        for (size_type u = 0; u < len; ++u)
            if (!uf.merge(u, nxt[u])) rot[u] = u;

        for (size_type u = 0; u < len; ++u)
            if (rot[u] == u) rot[uf.find(u)] = u;

        for (size_type u = 0; u < len; ++u) rot[u] = rot[uf.find(u)];

        std::vector<std::pair<size_type, size_type>> e;
        e.reserve(len);

        for (size_type u = 0; u < len; ++u)
            if (rot[u] == u)
                e.emplace_back(len, u);
            else
                e.emplace_back(nxt[u], u);

        static_csr g(len + 1, e);
        h.build(g, len);
    }

    [[nodiscard]] constexpr bool is_in_cycle(size_type u) const noexcept {
        assert(u < len);
        return h.is_ancestor(u, nxt[rot[u]]);
    }

    [[nodiscard]] constexpr size_type min_cycle(size_type u) const noexcept {
        size_type r = rot[u];

        size_type ans = h.dep[nxt[r]] - h.dep[r] + 1;
        if (!is_in_cycle(u)) ans += h.dep[u] - h.dep[h.lca(u, nxt[r])];

        return ans;
    }

    [[nodiscard]] constexpr std::optional<size_type> dist(size_type u,
                                                          size_type v) const noexcept {
        assert(u < len && v < len);
        if (h.is_ancestor(v, u)) return h.dep[u] - h.dep[v];

        size_type r = rot[u], b = nxt[r];
        if (!h.is_ancestor(v, b)) return std::nullopt;

        return h.dep[u] - h.dep[r] + h.dep[b] - h.dep[v] + 1;
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] constexpr size_type jump(size_type u, Scalar k) const noexcept {
        assert(u < len);
        if (k < h.dep[u]) return *h.jump(u, static_cast<size_type>(k));

        k -= h.dep[u] - 1, u = rot[u];
        k %= h.dep[nxt[u]];
        return k == 0 ? u : *h.jump(nxt[u], static_cast<size_type>(k) - 1);
    }
};
} // namespace mld

#endif // MALLARD_FUNCTIONAL_GRAPH_H
