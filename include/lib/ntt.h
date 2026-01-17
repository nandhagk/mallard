#ifndef MALLARD_NTT_H
#define MALLARD_NTT_H 1

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <vector>

#include "lib/prelude.h"

namespace mld {
template <bool inv, typename Z>
constexpr void ntt(std::vector<Z> &a) noexcept {
    std::array<Z, 30> dw{}, iw{};

    // TODO: Fix this
    Z root = 3;
    for (u32 i = 0; i < 30; ++i) {
        dw[i] = -root.pow((Z::mod() - 1) >> (i + 2));
        iw[i] = dw[i].inv();
    }

    u32 len = static_cast<u32>(a.size());
    assert(std::has_single_bit(len));

    if constexpr (!inv) {
        for (u32 m = len; m >>= 1;) {
            Z w = 1;
            for (u32 s = 0, k = 0; s < len; s += 2 * m) {
                for (u32 i = s, j = s + m; i < s + m; ++i, ++j) {
                    Z x = a[i], y = a[j] * w;
                    a[i] = x + y;
                    a[j] = x - y;
                }

                w *= dw[std::countr_zero(++k)];
            }
        }
    } else {
        for (u32 m = 1; m < len; m <<= 1) {
            Z w = 1;
            for (u32 s = 0, k = 0; s < len; s += 2 * m) {
                for (u32 i = s, j = s + m; i < s + m; ++i, ++j) {
                    Z x = a[i], y = a[j];
                    a[i] = x + y;
                    a[j] = (x - y) * w;
                }

                w *= iw[std::countr_zero(++k)];
            }
        }

        Z z = Z(len).inv();
        std::ranges::transform(a, a.begin(), [z](auto &&x) { return x * z; });
    }
}
} // namespace mld

#endif // MALLARD_NTT_H
