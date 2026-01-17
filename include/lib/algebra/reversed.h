#ifndef MALLARD_ALGEBRA_REVERSED_H
#define MALLARD_ALGEBRA_REVERSED_H 1

#include <type_traits>

#include "lib/algebra/base.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/internal/dummy.h"

namespace mld::algebra {
template <internal::magma M>
struct reversed : base<typename M::value_type>,
                  std::conditional_t<internal::associative<M>, associative,
                                     mld::internal::dummy<1>>,
                  std::conditional_t<internal::commutative<M>, commutative<reversed<M>>,
                                     mld::internal::dummy<2>>,
                  std::conditional_t<internal::idempotent<M>, idempotent<reversed<M>>,
                                     mld::internal::dummy<3>> {
    using operand = base<typename M::value_type>;

    using operand::operand;
    using operand::val;

    constexpr reversed() noexcept
        : reversed(M().val()) {}

    [[nodiscard]] friend constexpr reversed operator+(const reversed &lhs,
                                                      const reversed &rhs) noexcept {
        return (static_cast<M>(rhs.val()) + static_cast<M>(lhs.val())).val();
    }

    template <std::unsigned_integral Scalar>
        requires(!internal::idempotent<reversed>)
    [[nodiscard]] friend constexpr reversed operator*(const reversed &lhs,
                                                      Scalar n) noexcept {
        return (static_cast<M>(lhs.val()) * n).val();
    }

    [[nodiscard]] constexpr reversed operator+() const noexcept
        requires(!internal::commutative<reversed>)
    {
        return (+static_cast<M>(val())).val();
    }

    [[nodiscard]] constexpr reversed operator-() const noexcept
        requires internal::invertible<M>
    {
        return (-static_cast<M>(val())).val();
    }

    [[nodiscard]] constexpr operator bool() const noexcept {
        return static_cast<bool>(static_cast<M>(val()));
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_REVERSED_H
