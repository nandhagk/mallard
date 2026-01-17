#ifndef MALLARD_OFFLINE_RECTANGLE_UNION_H
#define MALLARD_OFFLINE_RECTANGLE_UNION_H 1

#include <numeric>
#include <type_traits>
#include <vector>

#include "lib/actions/add_min_cnt.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/type_traits/numeric.h"
// #include "ska_sort.h"

namespace mld::offline {
template <typename T, typename U = make_double_width_t<T>>
struct rectangle_union {
    using size_type = u32;
    using value_type = T;
    using area_type = U;

private:
    std::vector<std::pair<value_type, size_type>> u, v;

public:
    constexpr rectangle_union() noexcept = default;

    constexpr void reserve(size_type n) noexcept {
        u.reserve(2 * n);
        v.reserve(2 * n);
    }

    constexpr void add_rectangle(value_type x0, value_type y0, value_type x1,
                                 value_type y1) noexcept {
        size_type n = static_cast<size_type>(u.size());
        u.emplace_back(x0, n), v.emplace_back(y0, n), ++n;
        u.emplace_back(x1, n), v.emplace_back(y1, n);
    }

    constexpr area_type area() noexcept {
        assert(u.size() >= 2);
        std::ranges::sort(u), std::ranges::sort(v);
        // ska::ska_sort(u.begin(), u.end()), ska::ska_sort(v.begin(), v.end());

        std::vector<size_type> w(v.size());
        for (size_type i = 0; i < v.size(); ++i) w[v[i].second] = i;

        using action = actions::add_min_cnt<std::make_signed_t<size_type>, value_type>;
        using operand = action::operand;
        lazy_segment_tree<action> s(std::views::iota(size_type{0}, v.size() - 1) |
                                    std::views::transform([&](size_type k) {
                                        return operand(0, v[k + 1].first - v[k].first);
                                    }));

        area_type ans = 0;
        value_type tot = v.back().first - v.front().first;
        for (size_type i = 0; i < u.size() - 1; ++i) {
            size_type k = u[i].second / 2;

            auto [l, r] = std::minmax(w[2 * k], w[2 * k + 1]);
            s.apply(l, r, u[i].second % 2 ? -1 : 1);

            auto [m, c] = s.fold().val();
            ans += static_cast<area_type>(u[i + 1].first - u[i].first) *
                   (tot - (m == 0 ? c : 0));
        }

        return ans;
    }
};
} // namespace mld::offline

#endif // MALLARD_OFFLINE_RECTANGLE_UNION_H
