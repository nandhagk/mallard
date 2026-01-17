#ifndef MALLARD_ALGEBRA_BASE_H
#define MALLARD_ALGEBRA_BASE_H 1

#include <algorithm>
#include <concepts>

#include "lib/math.h"

namespace mld::algebra {

template <typename T>
struct base {
    using value_type = T;

protected:
    value_type v;

public:
    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    base(Args &&...args) noexcept
        : v(std::forward<Args>(args)...) {}

    [[nodiscard]] constexpr auto val() const noexcept {
        return v;
    }

    [[nodiscard]] explicit constexpr operator value_type() const noexcept {
        return val();
    }

    [[nodiscard]] friend constexpr auto operator<=>(const base &lhs,
                                                    const base &rhs) noexcept {
        return lhs.val() <=> rhs.val();
    }

    [[nodiscard]] friend constexpr bool operator==(const base &lhs,
                                                   const base &rhs) noexcept {
        return lhs.val() == rhs.val();
    }
};

struct associative {};

template <typename Derived>
struct commutative {
    [[nodiscard]] friend constexpr Derived operator+(const Derived &lhs) noexcept {
        return lhs;
    }
};

template <typename Derived>
struct scalar_multipliable {
    struct identity {
        template <std::unsigned_integral Scalar>
        [[nodiscard]] friend constexpr Derived operator*(const Derived &lhs,
                                                         Scalar) noexcept {
            return lhs;
        }
    };

    struct automatic {
        template <std::unsigned_integral Scalar>
        [[nodiscard]] friend constexpr Derived operator*(const Derived &lhs,
                                                         Scalar n) noexcept {
            return pow<Derived, Scalar, std::plus<Derived>>(lhs, n, {}, {});
        }
    };
};

template <typename Derived>
struct idempotent : scalar_multipliable<Derived>::identity {};

template <typename Derived>
struct truthy {
    [[nodiscard]] explicit constexpr operator bool() const noexcept {
        return *static_cast<const Derived *>(this) != Derived();
    }
};

} // namespace mld::algebra

#endif // MALLARD_ALGEBRA_BASE_H
