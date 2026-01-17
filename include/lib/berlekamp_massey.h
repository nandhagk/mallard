#ifndef MALLARD_BERLEKAMP_MASSEY_H
#define MALLARD_BERLEKAMP_MASSEY_H 1

#include <vector>

#include "lib/prelude.h"

namespace mld {
template <typename Z>
[[nodiscard]] constexpr std::vector<Z>
berlekamp_massey(const std::vector<Z> &a) noexcept {
    u32 len = static_cast<u32>(a.size());
    std::vector<Z> s(a), c{1}, b{1};

    u32 m = 1;
    Z w = 1;
    for (u32 i = 0; i < len; ++i) {
        Z d = 0;
        for (u32 j = 0; j < c.size(); ++j) d += c[j] * s[i - j];

        if (d == 0) {
            ++m;
        } else if (2 * (c.size() - 1) <= i) {
            auto t = c;
            Z coef = d / w;

            c.resize(std::max(c.size(), b.size() + m));
            for (u32 j = 0; j < b.size(); ++j) c[j + m] -= coef * b[j];

            b = t, w = d, m = 1;
        } else {
            Z coef = d / w;

            c.resize(std::max(c.size(), b.size() + m));
            for (u32 j = 0; j < b.size(); ++j) c[j + m] -= coef * b[j];

            ++m;
        }
    }

    c.erase(c.begin());
    for (u32 i = 0; i < c.size(); ++i) c[i] = -c[i];

    return c;
}
} // namespace mld

#endif // MALLARD_BERLEKAMP_MASSEY_H
