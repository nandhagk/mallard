#ifndef MALLARD_DYNAMIC_GRAPH_H
#define MALLARD_DYNAMIC_GRAPH_H 1

#include <cassert>
#include <concepts>
#include <vector>

#include "lib/dynamic_csr.h"
#include "lib/prelude.h"

namespace mld {
template <typename Edge>
struct dynamic_graph : dynamic_csr<Edge> {
    using base = dynamic_csr<Edge>;

    using typename base::size_type;
    using typename base::value_type;

    using base::base;

    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    constexpr void add_edge(size_type u, Args &&...args) noexcept {
        return base::operator[](u).emplace_back(std::forward<Args>(args)...);
    }
};
} // namespace mld

#endif // MALLARD_DYNAMIC_GRAPH_H
