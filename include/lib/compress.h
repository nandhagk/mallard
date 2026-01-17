#ifndef MALLARD_COMPRESS_H
#define MALLARD_COMPRESS_H 1

#include <algorithm>
#include <ranges>
#include <type_traits>
#include <vector>

#include "lib/prelude.h"
// #include "ska_sort.h"

namespace mld {

template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::input_range<R>
[[nodiscard]] constexpr auto compress(R &&r) noexcept {
    using value_type = std::remove_cvref_t<std::ranges::range_value_t<R>>;
    u32 len = static_cast<u32>(std::ranges::size(r));

    std::vector<std::pair<value_type, u32>> p;
    p.reserve(len);
    for (auto &&a : r) p.emplace_back(a, p.size());
    std::ranges::sort(p);
    // ska::ska_sort(p.begin(), p.end());

    std::vector<value_type> v;
    v.reserve(len);

    std::vector<u32> na(len);
    for (auto &&[u, k] : p) {
        if (v.empty() || v.back() != u) v.push_back(u);
        na[k] = static_cast<u32>(v.size() - 1);
    }

    return std::make_pair(std::move(v), std::move(na));
}
} // namespace mld

#endif // MALLARD_COMPRESS_H
