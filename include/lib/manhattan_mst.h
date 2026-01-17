#ifndef MALLARD_MANHATTAN_MST_H
#define MALLARD_MANHATTAN_MST_H 1

#include <algorithm>
#include <concepts>
#include <map>
#include <numeric>
#include <ranges>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename R, std::invocable<u32, u32> F>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R>
constexpr void manhattan_mst(R &&r, F &&f) noexcept {
    using point = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    using value_type =
        std::remove_cvref_t<decltype(std::get<0>(std::declval<point>()))>;
    static_assert(std::signed_integral<value_type>);

    u32 len = static_cast<u32>(std::ranges::size(r));
    if (len == 0) return;

    std::vector<u32> idx(len);
    std::iota(idx.begin(), idx.end(), 0);

    auto u = [](const point &p) {
        auto &&[x, y] = p;
        return x + y;
    };

    auto v = [](const point &p) {
        auto &&[x, y] = p;
        return x - y;
    };

    auto w = [](const point &p) {
        auto &&[x, _] = p;
        return x;
    };

    std::map<value_type, u32> active;
    for (u32 rot = 0; rot < 4; ++rot) {
        switch (rot) {
        case 0:
            std::ranges::sort(idx, [&](u32 i, u32 j) { return u(r[i]) < u(r[j]); });
            break;
        case 2:
            for (auto &&[x, _] : r) x = -x;
            std::ranges::sort(idx, [&](u32 i, u32 j) { return u(r[i]) < u(r[j]); });
            break;
        case 1:
        case 3:
            for (auto &&[x, y] : r) std::swap(x, y);
            break;
        default:
            break;
        }

        for (u32 i : idx) {
            auto end = active.upper_bound(w(r[i]));
            auto cur = end;
            for (; cur != active.begin();) {
                auto prv = std::prev(cur);

                u32 j = prv->second;
                if (v(r[i]) > v(r[j])) break;

                f(i, j), cur = prv;
            }

            active.erase(cur, end);
            active.emplace(w(r[i]), i);
        }

        active.clear();
    }
}
} // namespace mld

#endif // MALLARD_MANHATTAN_MST_H
