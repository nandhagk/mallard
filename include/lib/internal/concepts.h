#ifndef MALLARD_INTERNAL_CONCEPTS_H
#define MALLARD_INTERNAL_CONCEPTS_H 1

#include <concepts>
#include <ranges>
#include <type_traits>

namespace mld::internal {

template <typename V, typename R>
concept sized_range_of = std::ranges::sized_range<R> &&
                         std::convertible_to<std::ranges::range_value_t<R>, V>;

template <typename V, typename R>
concept sized_input_range_of = sized_range_of<V, R> && std::ranges::input_range<R>;

template <typename Graph>
using graph_weight_t =
    std::remove_cvref_t<decltype(std::get<1>(*std::declval<Graph>()[0].begin()))>;

template <typename G>
concept weighted_graph = requires { typename graph_weight_t<G>; };

} // namespace mld::internal

#endif // MALLARD_INTERNAL_CONCEPTS_H
