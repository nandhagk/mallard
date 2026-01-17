#ifndef MALLARD_DYNAMIC_MST_H
#define MALLARD_DYNAMIC_MST_H 1

#include <algorithm>
#include <functional>
#include <limits>

#include "lib/pbds.h"
#include "lib/prelude.h"
#include "lib/union_find_rollback.h"

namespace mld::offline {
template <typename T>
struct dynamic_mst {
    using weight_type = T;
    using size_type = u32;

    using query_type = std::function<void(weight_type, size_type)>;

private:
    size_type len;

    struct edge {
        size_type l, r;
        size_type u, v;
        weight_type w;

        constexpr edge() noexcept = default;

        constexpr edge(size_type a, size_type b, weight_type c, size_type p,
                       size_type q = std::numeric_limits<size_type>::max())
            : l(p), r(q), u(a), v(b), w(c) {}
    };

    std::vector<edge> x;
    pbds::hash_map<std::pair<size_type, size_type>, size_type> f;
    std::vector<std::pair<size_type, query_type>> q;

    std::vector<bool> ful, skp;
    union_find_rollback ufr;

    constexpr void solve(size_type l, size_type r, size_type &k, std::vector<edge> &E,
                         weight_type ans) noexcept {
        ful.assign(E.size(), false), skp.assign(E.size(), false);

        if (l + 1 == r) {
            for (auto &&e : E)
                if (ufr.merge(e.u, e.v)) ans += e.w;

            ufr.rollback_all();
            E.clear(), E.shrink_to_fit();

            for (; k < q.size() && q[k].first == r; ++k) q[k].second(ans, k);
            return;
        }

        for (size_type i = 0; i < E.size(); ++i) {
            auto &&e = E[i];
            if (e.l <= l && r <= e.r) {
                if (ufr.merge(e.u, e.v)) ful[i] = true, ans += e.w;
            } else {
                ufr.merge(e.u, e.v);
            }
        }

        ufr.rollback_all();
        for (size_type i = 0; i < E.size(); ++i) {
            auto &&e = E[i];
            if (e.l <= l && r <= e.r) {
                if (!ufr.merge(e.u, e.v)) skp[i] = true;
            }
        }

        ufr.rollback_all();
        for (size_type i = 0; i < E.size(); ++i) {
            auto &&e = E[i];
            if (ful[i]) ufr.merge(e.u, e.v);
        }

        size_type m = std::midpoint(l, r);

        std::vector<edge> EL, ER;
        EL.reserve(E.size()), ER.reserve(E.size());

        for (size_type i = 0; i < E.size(); ++i) {
            auto &&e = E[i];
            if (skp[i] || ful[i]) continue;

            size_type u = ufr.find(e.u), v = ufr.find(e.v);
            if (u == v) continue;

            if (e.l < m && l < e.r) EL.emplace_back(u, v, e.w, e.l, e.r);
            if (e.l < r && m < e.r) ER.emplace_back(u, v, e.w, e.l, e.r);
        }

        ufr.rollback_all();
        E.clear(), E.shrink_to_fit();

        solve(l, m, k, EL, ans);
        solve(m, r, k, ER, ans);
    }

public:
    dynamic_mst() noexcept = default;

    explicit dynamic_mst(size_type n) noexcept {
        build(n);
    }

    void build(size_type n) noexcept {
        len = n;
        x.clear(), f.clear(), q.clear();
    }

    void reserve(size_type m) noexcept {
        x.reserve(m);
        f.resize(m);
        q.reserve(m);
    }

    void update(size_type u, size_type v, weight_type w) noexcept {
        assert(u < len && v < len);
        if (u > v) std::swap(u, v);

        size_type k = static_cast<size_type>(x.size());
        if (auto it = f.find({u, v}); it != f.end()) x[it->second].r = k;

        x.emplace_back(u, v, w, f[{u, v}] = k);
    }

    template <typename F>
        requires std::convertible_to<F, query_type>
    constexpr void query(F &&z) noexcept {
        q.emplace_back(static_cast<size_type>(x.size()), std::forward<F>(z));
    }

    void solve() noexcept {
        std::ranges::sort(x, {}, &edge::w);

        size_type k = 0;
        for (; k < q.size() && q[k].first == 0; ++k) q[k].second(0, k);

        ufr.build(len), ufr.reserve(len);
        solve(0, static_cast<size_type>(x.size()), k, x, 0);
    }
};
} // namespace mld::offline

#endif // MALLARD_DYNAMIC_MST_H
