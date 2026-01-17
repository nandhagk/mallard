#ifndef MALLARD_AM_TREE_H
#define MALLARD_AM_TREE_H 1

#include <cassert>
#include <optional>
#include <utility>
#include <vector>

#include "lib/internal/am_tree.h"
#include "lib/managers/container.h"
#include "lib/prelude.h"

namespace mld {

template <typename T>
struct am_tree {
private:
    struct node;
    using container_type = std::vector<node>;

    using handler = managers::container::handler<container_type>;
    using tree = internal::am_tree<T, handler>;

public:
    using size_type = tree::size_type;
    using cost_type = tree::cost_type;
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
    constexpr am_tree() noexcept = default;

    explicit constexpr am_tree(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        cnt = len = n;
        buf.assign(len, node(true));
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

#endif // MALLARD_AM_TREE_H
