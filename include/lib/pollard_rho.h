#ifndef MALLARD_POLLARD_RHO_H
#define MALLARD_POLLARD_RHO_H 1

#include <random>
#include <ranges>
#include <vector>

#include "lib/dynamic_montgomery_modint.h"
#include "lib/miller_rabin.h"
#include "lib/prelude.h"
#include "lib/rand.h"

namespace mld {

template <std::unsigned_integral U>
[[nodiscard]] U pollard_rho(U n) noexcept {
    if (n % 2 == 0) return 2;
    if (miller_rabin(n)) return n;

    using Z = dynamic_montgomery_modint_base<U, -1>;
    Z::set_mod(n);

    Z R, one = 1;
    const auto f = [&](Z x) { return x * x + R; };

    std::uniform_int_distribution<U> rnd(2, n - 1);

    for (;;) {
        Z x, y, ys, q = one;

        R = rnd(MT);
        y = rnd(MT);

        U g = 1;
        constexpr static const u32 m = 128;

        for (u32 r = 1; g == 1; r <<= 1) {
            x = y;
            for (u32 i = 0; i < r; ++i) y = f(y);
            for (u32 k = 0; g == 1 && k < r; k += m) {
                ys = y;
                for (u32 i = 0; i < m && i < r - k; ++i) q *= x - (y = f(y));
                g = gcd(q.val(), n);
            }
        }

        if (g == n) {
            do {
                g = gcd((x - (ys = f(ys))).val(), n);
            } while (g == 1);
        }

        if (g != n) return g;
    }

    assert(0);
}

template <std::unsigned_integral U>
[[nodiscard]] std::vector<std::pair<U, u32>> factorize(U m) noexcept {
    std::vector<U> ps;
    auto dfs = [&](auto &&self, U n) -> void {
        if (n <= 1) return;

        if (U p = pollard_rho(n); p == n) {
            ps.push_back(p);
        } else {
            self(self, p);
            self(self, n / p);
        }
    };

    dfs(dfs, m);
    std::ranges::sort(ps);

    std::vector<std::pair<U, u32>> fs;
    for (auto &&p : ps)
        if (fs.empty() || fs.back().first != p)
            fs.emplace_back(p, 1);
        else
            ++fs.back().second;

    return fs;
}

} // namespace mld

#endif // MALLARD_POLLARD_RHO_H
