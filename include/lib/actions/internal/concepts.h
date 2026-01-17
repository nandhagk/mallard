#ifndef MALLARD_ACTIONS_INTERNAL_CONCEPTS_H
#define MALLARD_ACTIONS_INTERNAL_CONCEPTS_H 1

#include "lib/algebra/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::actions::internal {

template <typename T>
concept operatable =
    requires { typename T::operand; } && algebra::internal::magma<typename T::operand>;

template <typename T>
concept effective = requires { typename T::operation; } &&
                    algebra::internal::magma<typename T::operation> &&
                    requires(typename T::operation f, usize len) {
                        {
                            T::pow(f, len)
                        } -> std::convertible_to<typename T::operation>;
                    };

template <typename T>
concept action = operatable<T> && effective<T> &&
                 requires(typename T::operation f, typename T::operand x) {
                     { T::act(f, x) } -> std::convertible_to<typename T::operand>;
                 };

} // namespace mld::actions::internal

#endif // MALLARD_ACTIONS_INTERNAL_CONCEPTS_H
