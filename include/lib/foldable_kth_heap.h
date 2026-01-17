#ifndef MALLARD_FOLDABLE_KTH_HEAP_H
#define MALLARD_FOLDABLE_KTH_HEAP_H 1

#include <concepts>
#include <functional>

#include "lib/algebra/internal/concepts.h"
#include "lib/erasable_heap.h"
#include "lib/prelude.h"

namespace mld {
template <typename T, algebra::internal::abelian_group AbelianGroup, typename Compare = std::less<T>>
struct foldable_kth_heap {
    using value_type = T;
    using operand = AbelianGroup;
    using size_type = u32;

    using comparator = Compare;

private:
    using node = std::pair<value_type, operand>;

    struct node_comparator {
    public:
        constexpr node_comparator(const comparator &compare = comparator()) noexcept
            : cmp(compare) {};

        [[nodiscard]] constexpr bool operator()(const node &lhs,
                                                const node &rhs) const noexcept {
            return cmp(std::get<0>(lhs), std::get<0>(rhs));
        }

        [[nodiscard]] constexpr bool operator()(const value_type &lhs,
                                                const node &rhs) const noexcept {
            return cmp(lhs, std::get<0>(rhs));
        }

        [[nodiscard]] constexpr bool operator()(const node &lhs,
                                                const value_type &rhs) const noexcept {
            return cmp(std::get<0>(lhs), rhs);
        }

    private:
        [[no_unique_address]] comparator cmp;
    };

    struct reversed_node_comparator {
    public:
        constexpr reversed_node_comparator(
            const comparator &compare = comparator()) noexcept
            : cmp(compare) {};

        [[nodiscard]] constexpr bool operator()(const node &lhs,
                                                const node &rhs) const noexcept {
            return cmp(rhs, lhs);
        }

    private:
        [[no_unique_address]] node_comparator cmp;
    };

    size_type rnk;
    operand sum;

    [[no_unique_address]] node_comparator cmp;
    erasable_heap<node, reversed_node_comparator> hed;
    erasable_heap<node, node_comparator> tal;

    constexpr void flush() noexcept {
        for (; hed.size() < rnk && !tal.empty(); tal.pop()) {
            auto [v, x] = tal.top();
            sum = sum + x, hed.insert(v, x);
        }

        for (; hed.size() > rnk; hed.pop()) {
            auto [v, x] = hed.top();
            sum = sum + (-x), tal.insert(v, x);
        }
    }

public:
    constexpr foldable_kth_heap(size_type k,
                                const comparator &compare = comparator()) noexcept
        : rnk(k),
          cmp(compare),
          hed(reversed_node_comparator(compare)),
          tal(node_comparator(compare)) {}

    [[nodiscard]] constexpr size_type k() const noexcept {
        return rnk;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return hed.size() + tal.size();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return hed.empty() && tal.empty();
    }

    constexpr void increment() noexcept {
        ++rnk, flush();
    }

    constexpr void decrement() noexcept {
        --rnk, flush();
    }

    constexpr void insert(const value_type &v, const operand &x) noexcept {
        if (hed.size() < rnk) {
            sum = sum + x, hed.insert(v, x);
        } else if (!hed.empty() && cmp(hed.top(), v)) {
            auto [u, y] = hed.top();
            sum = sum + (-y), tal.insert(u, y), hed.pop();
            sum = sum + x, hed.insert(v, x);
        } else {
            tal.insert(v, x);
        }
    }

    constexpr void erase(const value_type &v, const operand &x) noexcept {
        if (hed.empty() || cmp(v, hed.top())) {
            tal.erase(v, x);
        } else {
            sum = sum + (-x), hed.erase(v, x);
            if (!tal.empty()) {
                auto [u, y] = tal.top();
                sum = sum + y, hed.insert(u, y), tal.pop();
            }
        }
    }

    [[nodiscard]] constexpr std::pair<value_type, operand> kth() noexcept {
        return {tal.top().first, sum};
    }
};
} // namespace mld

#endif // MALLARD_FOLDABLE_KTH_HEAP_H
