#ifndef MALLARD_OFFLINE_INCREMENTAL_SCC_H
#define MALLARD_OFFLINE_INCREMENTAL_SCC_H 1

#include <algorithm>
#include <functional>
#include <limits>
#include <vector>

#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/static_csr.h"

namespace mld::offline {

struct incremental_scc {
    using size_type = u32;

    using query_type = std::function<void(size_type)>;

private:
    static constexpr size_type UNSET = -1;

    size_type len;
    std::vector<std::pair<size_type, size_type>> edg;
    std::vector<size_type> tim, idx;
    std::vector<std::pair<size_type, query_type>> que;

    constexpr void
    solve(size_type l, size_type r,
          std::vector<std::tuple<size_type, size_type, size_type>> &E) noexcept {
        size_type m = std::midpoint(l, r);

        std::vector<std::pair<size_type, size_type>> h;
        h.reserve(r - l);

        size_type grp = 0;
        for (auto &&[k, u, v] : E) {
            if (r - l > 1 && k >= m) break;
            size_type &a = idx[u], &b = idx[v];
            if (a == UNSET) a = grp++;
            if (b == UNSET) b = grp++;
            h.emplace_back(a, b);
        }

        static_csr g(grp, h);
        auto [_, ids] = scc(g);

        if (r - l == 1) {
            for (auto &&[k, u, v] : E) {
                size_type a = idx[u], b = idx[v];
                if (ids[a] == ids[b]) tim[k] = l;
            }

            for (auto &&[_, u, v] : E) idx[u] = idx[v] = UNSET;
            E.clear(), E.shrink_to_fit();

            return;
        }

        std::vector<std::tuple<size_type, size_type, size_type>> EL, ER;
        EL.reserve(E.size()), ER.reserve(E.size());

        for (auto &&[k, u, v] : E) {
            size_type &a = idx[u], &b = idx[v];
            if (a == UNSET) a = grp++;
            if (b == UNSET) b = grp++;
            if (k < m) {
                if (ids[a] == ids[b])
                    EL.emplace_back(k, u, v);
                else
                    ER.emplace_back(k, ids[a], ids[b]);
            } else if (a < g.size() && b < g.size() && ids[a] == ids[b]) {
                tim[k] = k;
            } else {
                size_type p = a < g.size() ? ids[a] : a;
                size_type q = b < g.size() ? ids[b] : b;
                ER.emplace_back(k, p, q);
            }
        }

        for (auto &&[_, u, v] : E) idx[u] = idx[v] = UNSET;
        E.clear(), E.shrink_to_fit();

        if (!EL.empty()) solve(l, m, EL);
        if (!ER.empty()) solve(m, r, ER);
    };

public:
    constexpr incremental_scc() noexcept = default;

    explicit constexpr incremental_scc(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        edg.clear(), tim.clear(), idx.clear(), que.clear();
    }

    constexpr void reserve(size_type n) noexcept {
        edg.reserve(n), que.reserve(n);
    }

    constexpr void link(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        edg.emplace_back(u, v);
    }

    template <typename F>
        requires std::convertible_to<F, query_type>
    constexpr void query(F &&z) noexcept {
        que.emplace_back(static_cast<size_type>(edg.size()), std::forward<F>(z));
    }

    template <std::invocable<size_type, size_type> M>
    constexpr void solve(M &&m) noexcept {
        idx.assign(len, UNSET);
        tim.assign(edg.size(), std::numeric_limits<size_type>::max());

        std::vector<std::tuple<size_type, size_type, size_type>> E;
        E.reserve(edg.size());

        size_type k = 0;
        for (auto &&[u, v] : edg) E.emplace_back(k++, u, v);

        solve(0, static_cast<size_type>(edg.size()), E);

        std::vector<std::pair<size_type, size_type>> p;
        p.reserve(tim.size());
        for (size_type t : tim) p.emplace_back(t, p.size());
        std::ranges::sort(p);

        for (k = 0; k < que.size() && que[k].first == 0; ++k) que[k].second(k);
        for (size_type i = 0, j = 0; i < edg.size(); ++i) {
            for (; j < p.size() && p[j].first == i; ++j) {
                auto &&[u, v] = edg[p[j].second];
                m(u, v);
            }

            for (; k < que.size() && que[k].first == i + 1; ++k) que[k].second(k);
        }
    }
};
} // namespace mld::offline

#endif // MALLARD_OFFLINE_INCREMENTAL_SCC_H
