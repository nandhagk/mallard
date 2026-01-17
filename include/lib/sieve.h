#ifndef MALLARD_SIEVE_H
#define MALLARD_SIEVE_H 1

#include <cassert>
#include <numeric>
#include <type_traits>
#include <vector>

#include "lib/prelude.h"

namespace mld {

struct sieve {
    using size_type = u32;

private:
    size_type len;

public:
    std::vector<size_type> lpf, pri;

    constexpr sieve() noexcept = default;

    explicit constexpr sieve(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        pri.clear(), pri.reserve(len);
        lpf.assign(len + 1, 0);

        for (size_type i = 2; i <= len; ++i) {
            if (lpf[i] == 0) {
                lpf[i] = i;
                pri.push_back(i);
            }

            for (size_type j = 0; i * pri[j] <= len; ++j) {
                lpf[i * pri[j]] = pri[j];
                if (pri[j] == lpf[i]) break;
            }
        }
    }

    [[nodiscard]] constexpr bool is_prime(size_type k) const noexcept {
        assert(k <= len);
        return lpf[k] == k;
    }

    [[nodiscard]] constexpr std::vector<std::pair<size_type, size_type>>
    factorize(size_type k) const noexcept {
        assert(k <= len);

        std::vector<std::pair<size_type, size_type>> f;
        while (k != 1) {
            size_type p = lpf[k], c = 0;
            do {
                k /= p;
                ++c;
            } while (k % p == 0);

            f.emplace_back(p, c);
        }

        return f;
    }

    [[nodiscard]] constexpr size_type totient(size_type k) const noexcept {
        assert(k <= len);

        for (auto &&[p, _] : factorize(k)) k -= k / p;
        return k;
    }

    [[nodiscard]] constexpr auto mobius() const noexcept {
        std::vector<std::make_signed_t<size_type>> mob(len + 1);

        mob[1] = 1;
        for (size_type i = 2; i <= len; ++i)
            if (is_prime(i))
                mob[i] = -1;
            else if ((i / lpf[i]) % lpf[i])
                mob[i] = -1 * mob[i / lpf[i]];

        return mob;
    }

    [[nodiscard]] constexpr std::vector<size_type> totient() const noexcept {
        std::vector<size_type> tot(len + 1);
        std::iota(tot.begin(), tot.end(), 0);

        for (size_type p : pri)
            for (size_type j = p; j <= len; j += p) tot[j] -= tot[j] / p;

        return tot;
    }

    template <bool inv, typename Z>
    constexpr void gcd_transform(std::vector<Z> &a) const noexcept {
        size_type n = static_cast<size_type>(a.size()) - 1;
        assert(n <= len);

        for (size_type p : pri) {
            if (p > n) break;

            if constexpr (!inv)
                for (size_type i = n / p; i > 0; --i) a[i] += a[i * p];
            else
                for (size_type i = 1; i * p <= n; ++i) a[i] -= a[i * p];
        }
    }

    template <bool inv, typename Z>
    constexpr void lcm_transform(std::vector<Z> &a) const noexcept {
        size_type n = static_cast<size_type>(a.size()) - 1;
        assert(n <= len);

        for (size_type p : pri) {
            if (p > n) break;

            if constexpr (!inv)
                for (size_type i = 1; i * p <= n; ++i) a[i * p] += a[i];
            else
                for (size_type i = n / p; i > 0; --i) a[i * p] -= a[i];
        }
    }
};
} // namespace mld

#endif // MALLARD_SIEVE_H
