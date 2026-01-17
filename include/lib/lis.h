#ifndef MALLARD_LIS_H
#define MALLARD_LIS_H 1

#include <algorithm>
#include <concepts>
#include <ranges>
#include <type_traits>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::input_range<R>
[[nodiscard]] constexpr std::vector<u32> lis(R &&r) noexcept {
    using value_type = std::remove_cvref_t<std::ranges::range_value_t<R>>;

    std::vector<value_type> tab;
    std::vector<u32> idx;

    u32 len = static_cast<u32>(std::ranges::size(r));
    tab.reserve(len), idx.reserve(len);

    for (auto &&x : r) {
        if (auto it = std::ranges::lower_bound(tab, x); it == tab.end()) {
            tab.push_back(x);
            idx.push_back(static_cast<u32>(tab.size() - 1));
        } else {
            *it = x;
            idx.push_back(static_cast<u32>(it - tab.begin()));
        }
    }

    std::vector<u32> seq(tab.size());
    for (u32 i = static_cast<u32>(idx.size()), cur = static_cast<u32>(tab.size() - 1);
         i--;) {
        if (idx[i] == cur) {
            seq[cur] = i;
            if (cur == 0) break;
            --cur;
        }
    }

    return seq;
}
} // namespace mld

#endif // MALLARD_LIS_H
