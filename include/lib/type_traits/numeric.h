#ifndef MALLARD_NUMERIC_TYPE_TRAITS_H
#define MALLARD_NUMERIC_TYPE_TRAITS_H

#include "lib/prelude.h"

#include <concepts>

namespace mld {

template <std::unsigned_integral>
struct make_double_width {};

template <>
struct make_double_width<u8> {
    using type = u16;
};

template <>
struct make_double_width<u16> {
    using type = u32;
};

template <>
struct make_double_width<u32> {
    using type = u64;
};

template <>
struct make_double_width<u64> {
    using type = u128;
};

template <std::unsigned_integral U>
using make_double_width_t = typename make_double_width<U>::type;

} // namespace mld

#endif // MALLARD_NUMERIC_TYPE_TRAITS_H
