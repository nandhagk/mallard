#ifndef MALLARD_WHT_H
#define MALLARD_WHT_H 1

#include <algorithm>
#include <bit>
#include <cassert>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <char type, bool inv, typename Z>
constexpr void wht(std::vector<Z> &a) noexcept {
    u32 len = static_cast<u32>(a.size());
    assert(std::has_single_bit(len));

    for (u32 k = 1; k < len; k <<= 1) {
        for (u32 s = 0; s < len; s += k << 1) {
            for (u32 i = s; i < s + k; ++i) {
                Z u = a[i], v = a[i + k];
                if constexpr (type == '^') {
                    a[i] = u + v;
                    a[i + k] = u - v;
                }

                if constexpr (type == '|') {
                    if constexpr (inv)
                        a[i + k] -= u;
                    else
                        a[i + k] += u;
                }

                if constexpr (type == '&') {
                    if constexpr (inv)
                        a[i] -= v;
                    else
                        a[i] += v;
                }
            }
        }
    }

    if constexpr (type == '^' && inv) {
        Z z = Z(len).inv();
        std::ranges::transform(a, a.begin(), [z](auto &&x) { return x * z; });
    }
}
} // namespace mld

#endif // MALLARD_WHT_H
