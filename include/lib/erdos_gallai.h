#ifndef MALLARD_ERDOS_GALLAI_H
#define MALLARD_ERDOS_GALLAI_H 1

#include <vector>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

/*
 * https://networkx.org/documentation/stable/_modules/networkx/algorithms/
 * graphical.html#is_valid_degree_sequence_erdos_gallai
 */
template <typename R>
    requires internal::sized_input_range_of<u32, R>
[[nodiscard]] constexpr bool erdos_gallai(R &&r) noexcept {
    u32 len = static_cast<u32>(std::ranges::size(r));

    u64 min = len, max = 0, sum = 0, n = 0;
    std::vector<u64> deg(len);
    for (auto &&a : r) {
        if (a >= len) return false;
        if (a == 0) continue;

        ++deg[a];
        min = std::min<u64>(min, a);
        max = std::max<u64>(max, a);
        sum += a;
        ++n;
    }

    if (sum % 2 || sum > n * (n - 1)) return false;
    if (n == 0 || 4 * min * n >= (max + min + 1) * (max + min + 1)) return true;

    u64 k = 0, sum_deg = 0, sum_nj = 0, sum_jnj = 0;
    for (u64 dk = max; dk >= min; --dk) {
        if (dk < k + 1) return true;
        if (deg[dk] == 0) continue;
        u64 run_size = deg[dk];
        if (dk < k + run_size) run_size = dk - k;
        sum_deg += run_size * dk;
        for (u64 v = 0; v < run_size; ++v) {
            sum_nj += deg[k + v];
            sum_jnj += (k + v) * deg[k + v];
        }

        k += run_size;
        if (sum_deg > k * (n - 1) - k * sum_nj + sum_jnj) return false;
    }

    return true;
}
} // namespace mld

#endif // MALLARD_ERDOS_GALLAI_H
