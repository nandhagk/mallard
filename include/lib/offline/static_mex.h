#ifndef MALLARD_OFFLINE_STATIC_MEX_H
#define MALLARD_OFFLINE_STATIC_MEX_H 1

#include <iterator>
#include <ranges>
#include <type_traits>

#include "lib/algebra/min.h"
#include "lib/dynamic_csr.h"
#include "lib/internal/concepts.h"
#include "lib/segment_tree.h"

namespace mld::offline {
struct static_mex {
    using size_type = u32;
    using value_type = std::make_signed_t<size_type>;

private:
    size_type len, cap;
    std::vector<size_type> buf;
    dynamic_csr<std::pair<size_type, size_type>> que;

public:
    constexpr static_mex() noexcept = default;

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    explicit constexpr static_mex(R &&r) noexcept {
        build(std::forward<R>(r));
    }

    template <typename R>
        requires internal::sized_input_range_of<size_type, R>
    constexpr void build(R &&r) noexcept {
        len = static_cast<size_type>(std::ranges::size(r));
        cap = 0;

        buf.clear(), buf.reserve(len);
        for (auto &&x : r) buf.emplace_back(x);

        que.build(len);
    }

    constexpr void reserve(size_type n) noexcept {
        que.reserve(n);
    }

    constexpr void query(size_type l, size_type r) noexcept {
        assert(l < r && r <= len);
        que[r - 1].emplace_back(l, cap++);
    }

    [[nodiscard]] constexpr std::vector<size_type> solve() const noexcept {
        std::vector<size_type> ans(cap);

        segment_tree<algebra::min<value_type>> agg(
            std::views::iota(size_type{0}, len) |
            std::views::transform([](auto) { return -1; }));
        for (size_type r = 0; r < len; ++r) {
            if (buf[r] < len) agg.set(buf[r], r);
            for (auto &&[l, k] : que[r])
                ans[k] = agg.max_right(0, [&](auto &&x) { return x >= l; });
        }

        return ans;
    };
};

} // namespace mld::offline

#endif // MALLARD_STATIC_MEX_H
