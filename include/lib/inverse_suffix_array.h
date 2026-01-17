#ifndef MALLARD_INVERSE_SUFFIX_ARRAY_H
#define MALLARD_INVERSE_SUFFIX_ARRAY_H 1

#include <concepts>
#include <ranges>
#include <type_traits>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename R>
    requires std::ranges::sized_range<R> && std::ranges::random_access_range<R> &&
             std::unsigned_integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>
[[nodiscard]] constexpr std::vector<u32> inverse_suffix_array(R &&sa) noexcept {
    u32 len = static_cast<u32>(sa.size());

    std::vector<i32> rnk(len);
    for (u32 i = 0; i < len; ++i) rnk[sa[i]] = i;

    std::vector<u32> s(len);
    for (u32 k = 1; k < len; ++k) {
        u32 i = sa[k - 1], j = sa[k];
        s[j] = s[i];
        if (i < len - 1 && (j == len - 1 || (rnk[i + 1] > rnk[j + 1]))) ++s[j];
    }

    return s;
}
} // namespace mld

#endif // MALLARD_INVERSE_SUFFIX_ARRAY_H
