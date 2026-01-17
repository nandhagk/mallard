#ifndef MALLARD_FOLDABLE_AM_TREE_H
#define MALLARD_FOLDABLE_AM_TREE_H 1

#include <cassert>
#include <optional>
#include <utility>
#include <vector>

#include "lib/algebra/internal/concepts.h"
#include "lib/internal/concepts.h"
#include "lib/internal/foldable_am_tree.h"
#include "lib/managers/container.h"
#include "lib/prelude.h"

namespace mld {

template <typename T, algebra::internal::abelian_group Group>
struct foldable_am_tree {
private:
    struct node;
    using container_type = std::vector<node>;

    using handler = managers::container::handler<container_type>;
    using tree = internal::foldable_am_tree<T, Group, handler>;

public:
    using size_type = tree::size_type;
    using cost_type = tree::cost_type;
    using operand = tree::operand;
    using result = tree::result;

private:
    struct node : tree::node {
        using base = tree::node;
        using base::base;
    };

    [[no_unique_address]] handler hlr;
    size_type len, cnt;
    container_type buf;

public:
    constexpr foldable_am_tree() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    explicit constexpr foldable_am_tree(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<operand, R>
    constexpr void build(R &&r) noexcept {
        cnt = len = static_cast<size_type>(std::ranges::size(r));
        buf.clear();

        buf.reserve(len);
        for (auto &&a : r) buf.emplace_back(a);
    }

    constexpr result insert(size_type u, size_type v, cost_type w) noexcept {
        assert(u < len && v < len);

        auto res = tree::insert(&buf[u], &buf[v], w);
        cnt -= (res && !res->has_value());

        return res;
    }

    [[nodiscard]] constexpr bool is_connected(size_type u, size_type v) noexcept {
        assert(u < len && v < len);
        return tree::is_connected(&buf[u], &buf[v]);
    }

    constexpr void add(size_type u, const operand &x) noexcept {
        assert(u < len);
        tree::add(&buf[u], x);
    }

    [[nodiscard]] constexpr operand fold(size_type u) noexcept {
        assert(u < len);
        return tree::root(&buf[u])->val;
    }

    [[nodiscard]] constexpr size_type size(size_type u) noexcept {
        assert(u < len);
        return tree::root(&buf[u])->len;
    }

    [[nodiscard]] constexpr std::optional<cost_type> max_path(size_type u,
                                                              size_type v) noexcept {
        assert(u < len && v < len);
        return tree::max_path(&buf[u], &buf[v]);
    }

    constexpr result erase(size_type u, size_type v, cost_type w) noexcept {
        assert(u < len && v < len);

        auto res = tree::erase(&buf[u], &buf[v], w);
        cnt += res.has_value();

        return res;
    }

    [[nodiscard]] constexpr size_type ccs() const noexcept {
        return cnt;
    }
};

} // namespace mld

#endif // MALLARD_FOLDABLE_AM_TREE_H
