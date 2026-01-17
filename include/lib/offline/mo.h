#ifndef MALLARD_OFFLINE_MO_H
#define MALLARD_OFFLINE_MO_H 1

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <ranges>
#include <vector>

#include "lib/mo_order.h"
#include "lib/prelude.h"

namespace mld::offline {
struct mo {
    using size_type = u32;

private:
    size_type len;
    std::vector<std::pair<size_type, size_type>> que;

public:
    constexpr mo() noexcept = default;

    explicit constexpr mo(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        que.clear();
    }

    constexpr void reserve(size_type n) noexcept {
        que.reserve(n);
    }

    constexpr void query(size_type l, size_type r) noexcept {
        assert(l < r && r <= len);
        que.emplace_back(l, r);
    }

    template <std::invocable<size_type> A, std::invocable<size_type> R,
              std::invocable<size_type> ASK>
    constexpr void solve(A &&add, R &&rem, ASK &&ask) noexcept {
        solve(std::forward<A>(add), std::forward<A>(add), std::forward<R>(rem),
              std::forward<R>(rem), std::forward<ASK>(ask));
    }

    template <std::invocable<size_type> AL, std::invocable<size_type> AR,
              std::invocable<size_type> RL, std::invocable<size_type> RR,
              std::invocable<size_type> ASK>
    constexpr void solve(AL &&addl, AR &&addr, RL &&reml, RR &&remr,
                         ASK &&ask) noexcept {
        size_type l = 0, r = 0;
        for (size_type k : mo_order<u32>(que, len)) {
            auto &&[a, b] = que[k];
            while (l > a) addl(--l);
            while (r < b) addr(r++);
            while (l < a) reml(l++);
            while (r > b) remr(--r);
            ask(k);
        }
    }
};
} // namespace mld::offline

#endif // MALLARD_OFFLINE_MO_H
