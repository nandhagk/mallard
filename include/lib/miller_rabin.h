#ifndef MALLARD_MILLER_RABIN_H
#define MALLARD_MILLER_RABIN_H 1

#include <algorithm>
#include <bit>
#include <functional>
#include <initializer_list>
#include <numeric>
#include <ranges>
#include <type_traits>

#include "lib/dynamic_montgomery_modint.h"
#include "lib/math.h"
#include "lib/prelude.h"

namespace mld {

template <std::unsigned_integral>
constexpr std::initializer_list<u64> miller_rabin_bases = {};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u8> = {2};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u16> = {2, 3};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u32> = {2, 7, 61};

template <>
constexpr std::initializer_list<u64> miller_rabin_bases<u64> = {
    2, 325, 9375, 28178, 450775, 9780504, 1795265022};

template <typename R>
concept miller_rabin_input_range =
    std::ranges::input_range<R> &&
    std::unsigned_integral<std::remove_cvref_t<std::ranges::range_value_t<R>>>;

template <std::unsigned_integral U, miller_rabin_input_range R>
[[nodiscard]] constexpr bool miller_rabin(U n, R &&r) noexcept {
    if (n < 64) return (u64{1} << n) & 0x28208a20a08a28ac;
    if (n % 2 == 0) return false;

    if (const U d = (n - 1) >> std::countr_zero(n - 1); std::is_constant_evaluated()) {
        return std::ranges::all_of(r, [&](auto &&a) {
            U b = static_cast<U>(a % n);
            if (b == 0) return true;

            U t = d;
            U y = mod_pow(b, d, n);
            for (; t != n - 1 && y != 1 && y != n - 1; y = mod_mul(y, y, n), t <<= 1);

            return (y == n - 1) || (t % 2);
        });
    } else {
        using Z = dynamic_montgomery_modint_base<U, -1>;
        Z::set_mod(n);

        return std::ranges::all_of(r, [&](auto &&a) {
            Z b = a;
            if (b == 0) return true;

            U t = d;
            Z y = b.pow(d);
            for (; t != n - 1 && y != 1 && y != n - 1; y *= y, t <<= 1);

            return (y == n - 1) || (t % 2);
        });
    }
}

template <std::unsigned_integral U>
[[nodiscard]] constexpr bool miller_rabin(U n) noexcept {
    return miller_rabin(n, miller_rabin_bases<U>);
}

template <std::unsigned_integral U, U m>
constexpr bool is_prime_v = miller_rabin(m);

static_assert(is_prime_v<u32, 998'244'353>);
static_assert(is_prime_v<u32, 1'000'000'007>);

} // namespace mld

#endif // MALLARD_MILLER_RABIN_H
