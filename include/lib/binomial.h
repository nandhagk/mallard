#ifndef MALLARD_BINOMIAL_H
#define MALLARD_BINOMIAL_H 1

#include <cassert>
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
};
} // namespace mld

#endif // MALLARD_BINOMIAL_H
