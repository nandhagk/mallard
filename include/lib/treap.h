#ifndef MALLARD_TREAP_H
#define MALLARD_TREAP_H 1

#include <ranges>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/rand.h"

namespace mld {

template <typename handler>
struct treap {
    using size_type = u32;

    using pointer = handler::pointer;

    struct node {
        pointer l = handler::nil, r = handler::nil;
        u64 pri = MT();
    };

    [[nodiscard]] static constexpr std::pair<pointer, pointer>
    split(pointer v, size_type k) noexcept {
        if (v == handler::nil) return {handler::nil, handler::nil};

        v->push();
        if (size_type len = v->l->len; k > len) {
            auto [l, r] = split(v->r, k - len - 1);
            v->r = l;
            v->pull();
            return {v, r};
        } else {
            auto [l, r] = split(v->l, k);
            v->l = r;
            v->pull();
            return {l, v};
        }
    }

    [[nodiscard]] static constexpr std::tuple<pointer, pointer, pointer>
    split(pointer v, size_type l, size_type r) noexcept {
        auto [a, b] = split(v, l);
        auto [c, d] = split(b, r - l);
        return {a, c, d};
    }

    [[nodiscard]] static constexpr pointer merge(pointer u, pointer v) noexcept {
        if (u == handler::nil) return v;
        if (v == handler::nil) return u;

        if (u->pri > v->pri) {
            u->push();
            u->r = merge(u->r, v);
            u->pull();
            return u;
        } else {
            v->push();
            v->l = merge(u, v->l);
            v->pull();
            return v;
        }
    }

    [[nodiscard]] static constexpr pointer merge(pointer a, pointer b,
                                                 pointer c) noexcept {
        return merge(a, merge(b, c));
    }

    template <typename R>
        requires internal::sized_input_range_of<pointer, R>
    [[nodiscard]] static constexpr pointer build(R &&r) noexcept {
        size_type len = static_cast<size_type>(std::ranges::size(r));
        if (len == 0) return handler::nil;

        std::vector<pointer> stk;
        stk.reserve(len);

        for (pointer v : r) {
            pointer lst = handler::nil;
            while (!stk.empty() && v->pri >= stk.back()->pri) {
                lst = stk.back();
                lst->pull();
                stk.pop_back();
            }

            v->l = lst;
            if (!stk.empty()) stk.back()->r = v;
            stk.push_back(v);
        }

        for (pointer v : stk | std::views::reverse) v->pull();
        return stk.front();
    }
};
} // namespace mld

#endif // MALLARD_TREAP_H
