#ifndef MALLARD_STATIC_MEX_H
#define MALLARD_STATIC_MEX_H 1

#include <ranges>
#include <type_traits>

#include "lib/algebra/min.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/internal/concepts.h"

namespace mld {
template <typename ClonableManager>
struct static_mex {
    using size_type = u32;
    using value_type = std::make_signed_t<size_type>;

private:
    using tree =
        mld::dynamic_segment_tree<mld::algebra::min<value_type>, ClonableManager>;
    size_type len;
    std::vector<tree> agg;

public:
    using allocator_type = tree::allocator_type;

    constexpr static_mex() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    explicit constexpr static_mex(
        R &&r, const allocator_type &alloc = allocator_type()) noexcept {
        build(std::forward<R>(r), alloc);
    }

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    constexpr void build(R &&r,
                         const allocator_type &alloc = allocator_type()) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        agg.clear(), agg.reserve(len + 1);

        agg.emplace_back(std::views::iota(u32{0}, len) |
                             std::views::transform([](auto) { return -1; }),
                         alloc);

        size_type k = 0;
        for (auto &&x : r) {
            agg.emplace_back(agg.back());
            if (x < len) agg.back().set(x, k);
            ++k;
        }
    }

    [[nodiscard]] constexpr size_type mex(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);
        return agg[r].max_right(0, [&](auto &&x) { return x >= l; });
    }
};

} // namespace mld

#endif // MALLARD_STATIC_MEX_H
