#ifndef MALLARDK_KTH_INTEGRAL_ROOT_H
#define MALLARDK_KTH_INTEGRAL_ROOT_H

#include <cmath>
#include <limits>

#include "lib/math.h"
#include "lib/prelude.h"

namespace mld {
[[nodiscard]] constexpr u64 kth_integral_root(u64 n, u64 k) noexcept {
    assert(k >= 1);
    if (n == 0 || n == 1 || k == 1) return n;
    if (k >= 64) return 1;
    if (k == 2) return static_cast<u64>(std::sqrtl(n));
    if (n == std::numeric_limits<u64>::max()) --n;

    auto mul = [](u64 a, u64 b) {
        return a <= std::numeric_limits<u64>::max() / b
                   ? a * b
                   : std::numeric_limits<u64>::max();
    };

    u64 res = static_cast<u64>(std::pow(n, std::nextafter(1 / f64(k), 0)));
    for (; mld::pow(res + 1, k, mul) <= n; ++res);
    return res;
}
} // namespace mld

#endif // MALLARDK_KTH_INTEGRAL_ROOT_H
