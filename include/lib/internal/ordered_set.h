#ifndef MALLARD_INTERNAL_ORDERED_SET_H
#define MALLARD_INTERNAL_ORDERED_SET_H 1

#include <array>
#include <cassert>
#include <numeric>
#include <optional>
#include <vector>
#include <utility>

#include "lib/internal/concepts.h"
#include "lib/prelude.h"

namespace mld::internal {

template <typename T, typename Manager, u32 leaf, u32 load>
struct ordered_set {
    using value_type = T;

    struct node;
    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using size_type = u32;
    using allocator_type = handler::allocator_type;

protected:
    [[no_unique_address]] handler hlr;

    struct ordered_set_small {
        size_type len = 0;
        std::array<value_type, leaf> buf;

        constexpr ordered_set_small() noexcept = default;

        [[nodiscard]] constexpr auto get(size_type k) const noexcept {
            return buf[k];
        }

        constexpr bool insert(const value_type &x) noexcept {
            size_type i = 0;

            while (i < len && buf[i] < x) ++i;
            if (i < len && buf[i] == x) return false;

            for (size_type j = len; j > i; --j) buf[j] = buf[j - 1];
            buf[i] = x;

            ++len;
            return true;
        }

        constexpr bool erase(const value_type &x) noexcept {
            size_type i = 0;

            while (i < len && buf[i] < x) ++i;
            if (i == len || buf[i] != x) return false;

            for (size_type j = i; j + 1 < len; ++j) buf[j] = buf[j + 1];

            --len;
            return true;
        }

        [[nodiscard]] constexpr ordered_set_small split_half() noexcept {
            constexpr size_type mid = leaf / 2;

            ordered_set_small x;
            for (size_type i = mid; i < leaf; ++i) x.buf[i - mid] = buf[i];
            x.len = leaf - mid, len = mid;

            return x;
        }

        [[nodiscard]] constexpr size_type
        lower_bound(const value_type &x) const noexcept {
            for (size_type i = 0; i < len; ++i)
                if (buf[i] >= x) return i;

            return len;
        }

        [[nodiscard]] constexpr size_type
        upper_bound(const value_type &x) const noexcept {
            for (size_type i = 0; i < len; ++i)
                if (buf[i] > x) return i;

            return len;
        }

        [[nodiscard]] constexpr auto min() const noexcept {
            return buf[0];
        }

        [[nodiscard]] constexpr auto max() const noexcept {
            return buf[len - 1];
        }
    };

public:
    struct node {
        pointer l = handler::nil, r = handler::nil;
        size_type len = 0;
        ordered_set_small x;
        i32 h = 0;

        constexpr node() noexcept = default;

        explicit constexpr node(ordered_set_small z) noexcept
            : len(z.len), x(z), h(1) {}

        [[nodiscard]] constexpr i32 factor() const noexcept {
            return l->h - r->h;
        }

        constexpr void pull() noexcept {
            h = std::max(l->h, r->h) + 1;
            len = l->len + x.len + r->len;
        }
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            dispose(v->l);
            dispose(v->r);
            hlr.dispose(v);
        }
    }

    [[nodiscard]] static constexpr pointer rotate_right(pointer v) noexcept {
        pointer l = v->l;
        v->l = l->r;
        l->r = v;
        v->pull();
        l->pull();
        return l;
    }

    [[nodiscard]] static constexpr pointer rotate_left(pointer v) noexcept {
        pointer r = v->r;
        v->r = r->l;
        r->l = v;
        v->pull();
        r->pull();
        return r;
    }

    [[nodiscard]] static constexpr pointer balance(pointer v) noexcept {
        if (i32 bf = v->factor(); bf == 2) {
            if (v->l->factor() == -1) {
                v->l = rotate_left(v->l);
                v->pull();
            }
            return rotate_right(v);
        } else if (bf == -2) {
            if (v->r->factor() == 1) {
                v->r = rotate_right(v->r);
                v->pull();
            }
            return rotate_left(v);
        }
        return v;
    }

    [[nodiscard]] static constexpr pointer insert_leftmost(pointer v,
                                                           pointer u) noexcept {
        if (v == handler::nil) return u;
        v->l = insert_leftmost(v->l, u);
        v->pull();
        return balance(v);
    }

    [[nodiscard]] static constexpr pointer cut_leftmost(pointer v,
                                                        pointer &u) noexcept {
        if (v->l == handler::nil) {
            u = v;
            return v->r;
        }

        v->l = cut_leftmost(v->l, u);
        v->pull();
        return balance(v);
    }

    constexpr ordered_set(const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {}

private:
    template <std::input_iterator InputIt>
    [[nodiscard]] constexpr pointer build(InputIt &it, const InputIt &last, size_type s,
                                          size_type e) {
        size_type m = std::midpoint(s, e);

        pointer l = handler::nil, r = handler::nil;
        if (s < m) l = build(it, last, s, m);

        ordered_set_small x;
        for (; x.len < load && it != last; ++x.len, ++it) x.buf[x.len] = *it;
        pointer v = hlr.create(std::move(x));

        if (m + 1 < e) r = build(it, last, m + 1, e);

        v->l = l, v->r = r;
        v->pull();

        return v;
    }

public:
    template <typename R>
        requires internal::sized_input_range_of<value_type, R>
    [[nodiscard]] constexpr pointer build(R &&r) noexcept {
        size_type len = static_cast<size_type>(std::ranges::size(r));
        if (len == 0) return handler::nil;

        auto it = std::ranges::begin(r);
        return build(it, std::ranges::end(r), 0, (len + load - 1) / load);
    }

    [[nodiscard]] constexpr bool insert(pointer &v, const value_type &x) noexcept {
        if (v == handler::nil) {
            ordered_set_small y;
            y.buf[0] = x, y.len = 1;

            v = hlr.create(std::move(y));
            return true;
        }

        bool res = false;
        if (v->l != handler::nil && x < v->x.min()) {
            res = insert(v->l, x);
        } else if (v->r != handler::nil && x > v->x.max()) {
            res = insert(v->r, x);
        } else {
            res = v->x.insert(x);
            if (v->x.len == leaf) {
                pointer u = hlr.create(std::move(v->x.split_half()));
                v->r = insert_leftmost(v->r, u);
            }
        }

        v->pull(), v = balance(v);
        return res;
    }

    [[nodiscard]] constexpr bool erase(pointer &v, const value_type &x) noexcept {
        if (v == handler::nil) return false;

        bool res = false;
        if (x < v->x.min()) {
            res = erase(v->l, x);
        } else if (x > v->x.max()) {
            res = erase(v->r, x);
        } else {
            res = v->x.erase(x);
            if (v->x.len == 0) {
                if (v->r == handler::nil || v->l == handler::nil) {
                    pointer u = std::exchange(v, v->r == handler::nil ? v->l : v->r);
                    hlr.dispose(u);

                    return res;
                }

                pointer u = handler::nil, r = cut_leftmost(v->r, u);
                u->l = v->l, u->r = r;
                u->pull();

                u = std::exchange(v, balance(u));
                hlr.dispose(u);

                return res;
            }
        }

        v->pull(), v = balance(v);
        return res;
    }

    [[nodiscard]] constexpr bool contains(const pointer &u,
                                          const value_type &x) const noexcept {
        for (pointer v = u; v != handler::nil;) {
            if (x < v->x.min()) {
                v = v->l;
            } else if (x > v->x.max()) {
                v = v->r;
            } else {
                size_type idx = v->x.lower_bound(x);
                return idx < v->x.len && v->x.get(idx) == x;
            }
        }

        return false;
    }

    [[nodiscard]] constexpr std::optional<value_type>
    prev(const pointer &u, const value_type &x) const noexcept {
        value_type y = x;
        for (pointer v = u; v != handler::nil;) {
            if (x <= v->x.min()) {
                v = v->l;
            } else if (x > v->x.max()) {
                y = v->x.max();
                v = v->r;
            } else {
                size_type idx = v->x.lower_bound(x);
                return v->x.get(idx - 1);
            }
        }

        if (y == x) return std::nullopt;
        return y;
    }

    [[nodiscard]] constexpr std::optional<value_type>
    next(const pointer &u, const value_type &x) const noexcept {
        value_type y = x;
        for (pointer v = u; v != handler::nil;) {
            if (x < v->x.min()) {
                y = v->x.min();
                v = v->l;
            } else if (x >= v->x.max()) {
                v = v->r;
            } else {
                size_type idx = v->x.upper_bound(x);
                return v->x.get(idx);
            }
        }

        if (y == x) return std::nullopt;
        return y;
    }

    [[nodiscard]] constexpr value_type kth(const pointer &u,
                                           size_type k) const noexcept {
        for (pointer v = u; v != handler::nil;) {
            if (size_type len = v->l->len; k < len) {
                v = v->l;
            } else if (k < len + v->x.len) {
                return v->x.get(k - len);
            } else {
                k -= len + v->x.len;
                v = v->r;
            }
        }

        assert(0);
    }

    [[nodiscard]] constexpr size_type rank(const pointer &u,
                                           const value_type &r) const noexcept {
        size_type k = 0;
        for (pointer v = u; v != handler::nil;) {
            if (size_type len = v->l->len; r <= v->x.min()) {
                v = v->l;
            } else if (r <= v->x.max()) {
                k += len;
                return k + v->x.lower_bound(r);
            } else {
                k += len + v->x.len;
                v = v->r;
            }
        }

        return k;
    }

    template <std::invocable<value_type> F>
    constexpr void walk(const pointer &v, F &&f) const noexcept {
        if (v == handler::nil) return;

        if (v->l) walk(v->l, std::forward<F>(f));
        for (size_type i = 0; i < v->x.len; ++i) f(v->x.get(i));
        if (v->r) walk(v->r, std::forward<F>(f));
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_ORDERED_SET_H
