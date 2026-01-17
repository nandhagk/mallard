#ifndef MALLARD_RBST_H
#define MALLARD_RBST_H 1

#include <ranges>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"
#include "lib/rand.h"

namespace mld {

template <typename handler>
struct rbst {
    using size_type = u32;
    using pointer = handler::pointer;

    struct node {
        pointer l = handler::nil, r = handler::nil;
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

        if ((MT() % (u->len + v->len)) < u->len) {
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

private:
    template <std::input_iterator InputIt>
    [[nodiscard]] static constexpr pointer build(InputIt &it, size_type l,
                                                 size_type r) noexcept {
        size_type m = std::midpoint(l, r);

        pointer u = handler::nil, w = handler::nil;
        if (l < m) u = build(it, l, m);
        pointer v = *(it++);
        if (m + 1 < r) w = build(it, m + 1, r);

        v->l = u, v->r = w;
        v->pull();

        return v;
    }

public:
    template <typename R>
        requires internal::sized_input_range_of<pointer, R>
    [[nodiscard]] static constexpr pointer build(R &&r) noexcept {
        size_type len = static_cast<size_type>(std::ranges::size(r));
        if (len == 0) return handler::nil;

        auto it = std::ranges::begin(r);
        return build(it, 0, len);
    }
};
} // namespace mld

#endif // MALLARD_RBST_H
