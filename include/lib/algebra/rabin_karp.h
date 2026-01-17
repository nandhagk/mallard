#ifndef MALLARD_ALGEBRA_RABIN_KARP_H
#define MALLARD_ALGEBRA_RABIN_KARP_H 1

#include <concepts>

#include "lib/algebra/affine.h"
#include "lib/algebra/combined.h"
#include "lib/rand.h"

namespace mld::algebra {
template <typename... Zs>
struct rabin_karp : combined<affine<Zs>...> {
    using operand = combined<affine<Zs>...>;

    using operand::operand;
    using operand::val;

    template <std::integral T>
    explicit rabin_karp(T c) noexcept
        : operand(affine<Zs>(std::get<Zs>(base), c)...) {}

    static std::tuple<Zs...> base;

    static std::tuple<Zs...> rand() noexcept {
        return {static_cast<Zs>(MT())...};
    }

    [[nodiscard]] friend constexpr rabin_karp
    operator+(const rabin_karp &lhs, const rabin_karp &rhs) noexcept {
        return (static_cast<operand>(lhs) + static_cast<operand>(rhs)).val();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_RABIN_KARP_H
