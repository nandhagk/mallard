#ifndef MALLARD_KTH_HEAP_H
#define MALLARD_KTH_HEAP_H 1

#include <concepts>

#include "lib/erasable_heap.h"
#include "lib/prelude.h"

namespace mld {
template <typename T, typename Compare>
struct kth_heap {
    using value_type = T;
    using size_type = u32;

    using comparator = Compare;

private:
    struct reversed_comparator {
    public:
        constexpr reversed_comparator(const comparator &compare = comparator()) noexcept
            : cmp(compare) {};

        [[nodiscard]] constexpr bool operator()(const value_type &lhs,
                                                const value_type &rhs) const noexcept {
            return cmp(rhs, lhs);
        }

    private:
        [[no_unique_address]] comparator cmp;
    };

    size_type rnk;

    [[no_unique_address]] comparator cmp;
    erasable_heap<value_type, reversed_comparator> hed;
    erasable_heap<value_type, comparator> tal;

    constexpr void flush() noexcept {
        for (; hed.size() < rnk && !tal.empty(); tal.pop()) hed.insert(tal.top());
        for (; hed.size() > rnk; hed.pop()) tal.insert(hed.top());
    }

public:
    constexpr kth_heap(size_type k, const comparator &compare = comparator()) noexcept
        : rnk(k), cmp(compare), hed(reversed_comparator(compare)), tal(compare) {}

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

    constexpr void insert(const value_type &v) noexcept {
        if (hed.size() < rnk) {
            hed.insert(v);
        } else if (!hed.empty() && cmp(hed.top(), v)) {
            tal.insert(hed.top());
            hed.pop(), hed.insert(v);
        } else {
            tal.insert(v);
        }
    }

    constexpr void erase(const value_type &v) noexcept {
        if (hed.empty() || cmp(v, hed.top())) {
            tal.erase(v);
        } else {
            hed.erase(v);
            if (!tal.empty()) hed.insert(tal.top()), tal.pop();
        }
    }

    [[nodiscard]] constexpr auto kth() noexcept {
        return tal.top();
    }
};
} // namespace mld

#endif // MALLARD_KTH_HEAP_H
