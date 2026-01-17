#include <ctime>
#ifndef MALLARD_ALGEBRA_INTERNAL_CONCEPTS_H
#define MALLARD_ALGEBRA_INTERNAL_CONCEPTS_H 1

#include <concepts>

#include "lib/algebra/base.h"
#include "lib/internal/dummy.h"

namespace mld::algebra::internal {

template <typename T>
concept magma = requires { typename T::value_type; } && requires(T lhs, T rhs) {
    { lhs.val() } -> std::convertible_to<typename T::value_type>;
    { lhs + rhs } -> std::convertible_to<T>;
};

template <typename T>
concept associative = std::derived_from<T, algebra::associative>;

template <typename T>
concept commutative = std::derived_from<T, algebra::commutative<T>>;

template <typename T>
concept idempotent = std::derived_from<T, algebra::idempotent<T>>;

template <typename T>
concept semigroup = magma<T> && associative<T>;

template <typename T>
concept invertible = requires(T lhs) {
    { -lhs } -> std::convertible_to<T>;
};

template <typename T>
concept reversible = requires(T lhs) {
    { +lhs } -> std::convertible_to<T>;
};

template <typename T>
concept monoid = semigroup<T> && std::default_initializable<T>;

template <typename T>
concept abelian_monoid = monoid<T> && commutative<T>;

template <typename T>
concept group = monoid<T> && invertible<T>;

template <typename T>
concept abelian_group = group<T> && commutative<T>;

template <typename Aggregate>
concept foldable =
    requires {
        typename Aggregate::operand;
        typename Aggregate::size_type;
    } && monoid<typename Aggregate::operand> &&
    std::unsigned_integral<typename Aggregate::size_type> &&
    requires(Aggregate agg, Aggregate::size_type l, Aggregate::size_type r) {
        { agg.fold(l, r) } -> std::same_as<typename Aggregate::operand>;
    };

template <typename Aggregate>
concept appliable =
    requires {
        typename Aggregate::operation;
        typename Aggregate::size_type;
    } && monoid<typename Aggregate::operation> &&
    std::unsigned_integral<typename Aggregate::size_type> &&
    requires(Aggregate agg, Aggregate::size_type l, Aggregate::size_type r,
             Aggregate::operation f) {
        { agg.apply(l, r, f) } -> std::same_as<void>;
    };

template <typename T>
struct operation_type {
    using type = mld::internal::dummy<1>;
};

template <typename T>
    requires appliable<T>
struct operation_type<T> {
    using type = typename T::operation;
};

template <typename T>
using operation_type_t = operation_type<T>::type;

template <typename Aggregate, typename R>
concept buildable = requires(Aggregate agg, R r) {
    { agg.build(r) } -> std::same_as<void>;
};

} // namespace mld::algebra::internal

#endif // MALLARD_ALGEBRA_INTERNAL_CONCEPTS_H
