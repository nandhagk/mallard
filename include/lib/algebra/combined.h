#ifndef MALLARD_ALGEBRA_COMBINED_H
#define MALLARD_ALGEBRA_COMBINED_H 1

#include <type_traits>

#include "lib/algebra/base.h"
#include "lib/algebra/internal/concepts.h"
#include "lib/internal/dummy.h"

namespace mld::algebra {
template <internal::magma... Ms>
struct combined
    : base<std::tuple<typename Ms::value_type...>>,
      std::conditional_t<(internal::associative<Ms> && ...), associative,
                         mld::internal::dummy<1>>,
      std::conditional_t<(internal::commutative<Ms> && ...),
                         commutative<combined<Ms...>>, mld::internal::dummy<2>>,
      std::conditional_t<(internal::idempotent<Ms> && ...), idempotent<combined<Ms...>>,
                         mld::internal::dummy<3>> {
    using value_type = std::tuple<typename Ms::value_type...>;
    using operand = base<value_type>;

    using operand::operand;
    using operand::val;

    constexpr combined() noexcept
        : combined(Ms{}...) {}

    template <typename T>
        requires(std::convertible_to<T, typename Ms::value_type> && ...)
    constexpr explicit combined(const T &v) noexcept
        : combined(static_cast<Ms>(v)...){};

    [[nodiscard]] friend constexpr combined operator+(const combined &lhs,
                                                      const combined &rhs) noexcept {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::tuple{((static_cast<Ms>(std::get<I>(lhs.val())) +
                                static_cast<Ms>(std::get<I>(rhs.val())))
                                   .val())...};
        }(std::make_index_sequence<sizeof...(Ms)>{});
    }

    template <std::unsigned_integral Scalar>
        requires(!internal::idempotent<combined>)
    [[nodiscard]] friend constexpr combined operator*(const combined &lhs,
                                                      Scalar n) noexcept {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::tuple{((static_cast<Ms>(std::get<I>(lhs.val())) * n).val())...};
        }(std::make_index_sequence<sizeof...(Ms)>{});
    }

    [[nodiscard]] constexpr combined operator+() const noexcept
        requires(!internal::commutative<combined>)
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::tuple{((+static_cast<Ms>(std::get<I>(this->val()))).val())...};
        }(std::make_index_sequence<sizeof...(Ms)>{});
    }

    [[nodiscard]] constexpr combined operator-() const noexcept
        requires(internal::invertible<Ms> && ...)
    {
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::tuple{((-static_cast<Ms>(std::get<I>(this->val()))).val())...};
        }(std::make_index_sequence<sizeof...(Ms)>{});
    }

    [[nodiscard]] constexpr operator bool() const noexcept {
        return std::apply(
            [](auto &&...elems) -> bool {
                return (static_cast<bool>(static_cast<Ms>(elems)) || ...);
            },
            val());
    }
};
} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_COMBINED_H
