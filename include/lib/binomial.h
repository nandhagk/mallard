#ifndef MALLARD_BINOMIAL_H
#define MALLARD_BINOMIAL_H 1

#include <cassert>
#include <concepts>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <typename Z>
struct binomial {
    using value_type = Z;
    using size_type = u32;

private:
    size_type len;
    std::vector<value_type> fct, inv;

public:
    constexpr binomial() noexcept = default;

    explicit constexpr binomial(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        fct.resize(len + 1);
        inv.resize(len + 1);

        fct[0] = 1;
        for (size_type i = 1; i <= len; ++i) fct[i] = fct[i - 1] * i;

        inv[len] = fct[len].pow(Z::mod() - 2);
        for (size_type i = len; i != 0; --i) inv[i - 1] = inv[i] * i;
    }

    [[nodiscard]] constexpr value_type comb(size_type p, size_type q) const noexcept {
        assert(q <= p && p <= len);
        return fct[p] * inv[q] * inv[p - q];
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] constexpr value_type lucas(Scalar m, Scalar n) const noexcept {
        std::vector<size_type> a, b;
        size_type p = Z::mod();

        for (; m; m /= p) a.push_back(m % p);
        for (; n; n /= p) b.push_back(n % p);

        u32 k = std::max(static_cast<u32>(a.size()), static_cast<u32>(b.size()));
        a.resize(k, 0), b.resize(k, 0);

        Z res = 1;
        for (u32 i = 0; i < k; ++i) res *= a[i] < b[i] ? Z(0) : comb(a[i], b[i]);

        return res;
    }
};
} // namespace mld

#endif // MALLARD_BINOMIAL_H
