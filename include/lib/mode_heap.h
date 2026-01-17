#ifndef MALLARD_MODE_HEAP_H
#define MALLARD_MODE_HEAP_H 1

#include <functional>

#include "lib/erasable_heap.h"
#include "lib/pbds.h"
#include "lib/prelude.h"

namespace mld {
template <typename T, typename Compare = std::less<T>>
struct mode_heap {
    using value_type = T;
    using size_type = u32;

    using comparator = Compare;

private:
    using node = std::pair<size_type, value_type>;

    struct node_comparator {
    public:
        constexpr node_comparator(const comparator &compare = comparator()) noexcept
            : cmp(compare) {};

        [[nodiscard]] constexpr bool operator()(const node &lhs,
                                                const node &rhs) const noexcept {
            auto &&[a, b] = lhs;
            auto &&[p, q] = rhs;
            return a == p ? cmp(b, q) : a < p;
        }

    private:
        [[no_unique_address]] comparator cmp;
    };

    pbds::hash_map<value_type, size_type> cnt;
    erasable_heap<node, node_comparator> hep;

public:
    constexpr mode_heap(const comparator &compare = comparator()) noexcept
        : hep(compare) {}

    constexpr void reserve(size_type n) noexcept {
        cnt.resize(n);
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return hep.size();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return hep.empty();
    }

    constexpr void insert(const value_type &x) noexcept {
        if (cnt[x]) hep.erase(cnt[x], x);
        hep.insert(++cnt[x], x);
    }

    constexpr void erase(const value_type &x) noexcept {
        hep.erase(cnt[x], x);
        if (--cnt[x]) hep.insert(cnt[x], x);
    }

    [[nodiscard]] constexpr std::pair<value_type, size_type> mode() noexcept {
        auto &&[a, b] = hep.top();
        return {b, a};
    }
};
} // namespace mld

#endif // MALLARD_MODE_HEAP_H
