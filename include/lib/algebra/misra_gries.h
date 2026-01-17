#ifndef MALLARD_ALGEBRA_MISRA_GRIES_H
#define MALLARD_ALGEBRA_MISRA_GRIES_H 1

#include <vector>

#include "lib/algebra/base.h"
#include "lib/prelude.h"

namespace mld::algebra {
template <typename T, u32 K>
struct misra_gries : base<std::vector<std::pair<T, u32>>>,
                     associative,
                     commutative<misra_gries<T, K>>,
                     truthy<misra_gries<T, K>> {
    using operand = base<std::vector<std::pair<T, u32>>>;

    using operand::operand;
    using operand::val;

    explicit constexpr misra_gries(const T &t, u32 k = 1) noexcept
        : misra_gries(std::vector<std::pair<T, u32>>{{t, k}}) {}

    [[nodiscard]] friend constexpr misra_gries
    operator+(const misra_gries &lhs, const misra_gries &rhs) noexcept {
        std::vector<std::pair<T, u32>> z;
        z.reserve(2 * (K - 1));

        auto &&x = lhs.val();
        auto &&y = rhs.val();

        auto it = x.begin();
        auto ti = y.begin();

        while (it != x.end() && ti != y.end()) {
            auto &&[u, v] = *it;
            auto &&[p, q] = *ti;

            if (u == p) {
                z.emplace_back(u, v + q);
                ++it;
                ++ti;
            } else if (u < p) {
                z.emplace_back(u, v);
                ++it;
            } else {
                z.emplace_back(p, q);
                ++ti;
            }
        }

        for (; it != x.end(); ++it) z.emplace_back(*it);
        for (; ti != y.end(); ++ti) z.emplace_back(*ti);

        if (z.size() >= K) {
            std::vector<u32> f;
            f.reserve(z.size());

            for (auto &&[_, c] : z) f.push_back(c);

            usize a = f.size() - K;
            std::nth_element(f.begin(), f.begin() + a, f.end());

            u32 w = f[a];
            z.erase(std::remove_if(z.begin(), z.end(),
                                   [&](auto &&p) { return p.second <= w; }),
                    z.end());

            for (auto &&[_, c] : z) c -= w;
        }

        return z;
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_MISRA_GRIES_H
