#ifndef MALLARD_ALGEBRA_RABIN_KARP_REV_H
#define MALLARD_ALGEBRA_RABIN_KARP_REV_H 1

#include <concepts>

#include "lib/algebra/affine_rev.h"
#include "lib/algebra/combined.h"
#include "lib/rand.h"

namespace mld::algebra {
template <typename... Zs>
struct rabin_karp_rev : combined<affine_rev<Zs>...> {
    using operand = combined<affine_rev<Zs>...>;

    using operand::operand;
    using operand::val;

    template <std::integral T>
    explicit rabin_karp_rev(T c) noexcept
        : operand(affine_rev<Zs>(std::get<Zs>(base), c)...) {}

    static std::tuple<Zs...> base;

    static std::tuple<Zs...> rand() noexcept {
        return {static_cast<Zs>(MT())...};
    }

    [[nodiscard]] friend constexpr rabin_karp_rev
    operator+(const rabin_karp_rev &lhs, const rabin_karp_rev &rhs) noexcept {
        return (static_cast<operand>(lhs) + static_cast<operand>(rhs)).val();
    }

    [[nodiscard]] constexpr rabin_karp_rev operator+() const noexcept {
        return (+static_cast<operand>(*this)).val();
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_RABIN_KARP_REV_H
