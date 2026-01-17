#ifndef MALLARD_MO_ORDER_H
#define MALLARD_MO_ORDER_H 1

#include <concepts>
#include <ranges>
#include <vector>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {
template <std::unsigned_integral T, typename R>
    requires internal::sized_input_range_of<std::pair<T, T>, R>
[[nodiscard]] constexpr auto mo_order(R &&que, u32 n) noexcept {
    u32 len = static_cast<u32>(std::ranges::size(que));
    u32 blk = std::max<u32>(
        1, static_cast<u32>(std::sqrt(3) * n /
                            std::sqrt(std::max<u32>(1, 2 * static_cast<u32>(len)))));

    std::vector<std::tuple<T, T, u32>> ord;
    ord.reserve(len);

    for (auto &&[l, r] : que) {
        u32 a = l / blk;
        ord.emplace_back(a, ((a & 1) ? r : -r), ord.size());
    }

    std::ranges::sort(ord);

    auto r = ord | std::views::elements<2>;
    return std::vector<u32>(r.begin(), r.end());
}
} // namespace mld

#endif // MALLARD_MO_ORDER_H
