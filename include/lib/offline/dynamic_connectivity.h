#ifndef MALLARD_OFFLINE_DYNAMIC_CONNECTIVITY_H
#define MALLARD_OFFLINE_DYNAMIC_CONNECTIVITY_H 1

#include <concepts>
#include <functional>
#include <limits>
#include <vector>

#include "lib/am_tree.h"
#include "lib/pbds.h"

namespace mld::offline {
template <typename AMTree>
struct dynamic_connectivity {
    using tree = AMTree;

    using size_type = tree::size_type;
    using cost_type = tree::cost_type;
    static_assert(std::same_as<cost_type, i32>);

    using query_type = std::function<void(tree &, size_type)>;

private:
    size_type len;
    std::vector<std::tuple<size_type, size_type, cost_type>> e;
    pbds::hash_map<std::pair<size_type, size_type>, size_type> f;
    std::vector<std::pair<size_type, query_type>> q;

public:
    dynamic_connectivity() noexcept = default;

    explicit dynamic_connectivity(size_type n) noexcept {
        build(n);
    }

    void build(size_type n) noexcept {
        len = n;
        e.clear(), f.clear(), q.clear();
    }

    void reserve(size_type m) noexcept {
        e.reserve(m);
        f.resize(m);
        q.reserve(m);
    }

    void link(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        if (u > v) std::swap(u, v);

        f[{u, v}] = static_cast<size_type>(e.size());
        e.emplace_back(u, v, std::numeric_limits<cost_type>::lowest());
    };

    void cut(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        if (u > v) std::swap(u, v);

        std::get<2>(e[f[{u, v}]]) = -static_cast<cost_type>(e.size());
        e.emplace_back(u, v, 1);
    };

    template <typename F>
        requires std::convertible_to<F, query_type>
    constexpr void query(F &&z) noexcept {
        q.emplace_back(static_cast<size_type>(e.size()), std::forward<F>(z));
    }

    constexpr void solve(tree &amt) noexcept {
        size_type k = 0;

        for (size_type i = 0; i < e.size(); ++i) {
            for (; k < q.size() && q[k].first == i; ++k) q[k].second(amt, k);

            auto &&[u, v, w] = e[i];
            if (w == 1)
                amt.erase(u, v, -static_cast<cost_type>(i + 1));
            else
                amt.insert(u, v, w);
        }

        for (; k < q.size(); ++k) q[k].second(amt, k);
    }
};

} // namespace mld::offline

#endif // MALLARD_OFFLINE_DYNAMIC_CONNECTIVITY_H
