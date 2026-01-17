#ifndef MALLARD_ERASABLE_HEAP_H
#define MALLARD_ERASABLE_HEAP_H 1

#include <queue>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld {

template <typename T, typename Compare = std::less<T>>
struct erasable_heap {
    using value_type = T;
    using size_type = u32;

    using comparator = Compare;

private:
    [[no_unique_address]] comparator cmp;

    size_type len = 0;
    std::priority_queue<value_type, std::vector<value_type>, comparator> in, out;

    constexpr void flush() noexcept {
        while (!out.empty() && !cmp(out.top(), in.top()) && !cmp(in.top(), out.top()))
            in.pop(), out.pop();
    }

public:
    constexpr erasable_heap(const comparator &compare = comparator()) noexcept
        : cmp(compare), in(compare), out(compare) {}

    template <typename R>
        requires internal::sized_input_range_of<value_type, R>
    explicit constexpr erasable_heap(R &&r,
                                     const comparator &compare = comparator()) noexcept
        : cmp(compare),
          len(static_cast<size_type>(std::ranges::size(r))),
          in(std::ranges::begin(r), std::ranges::end(r), compare),
          out(compare) {}

    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    constexpr void insert(Args &&...args) noexcept {
        ++len, in.emplace(std::forward<Args>(args)...);
    }

    template <typename... Args>
        requires std::constructible_from<value_type, Args...>
    constexpr void erase(Args &&...args) noexcept {
        --len, out.emplace(std::forward<Args>(args)...);
    }

    [[nodiscard]] constexpr auto top() noexcept {
        flush();
        return in.top();
    }

    constexpr void pop() noexcept {
        flush();
        --len, in.pop();
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return len == 0;
    }
};

} // namespace mld

#endif // MALLARD_ERASABLE_HEAP_H
