#ifndef MALLARD_INTERNAL_BINARY_TRIE_H
#define MALLARD_INTERNAL_BINARY_TRIE_H 1

#include <algorithm>
#include <array>
#include <bit>
#include <concepts>
#include <limits>
#include <numeric>
#include <ranges>

#include "lib/prelude.h"

namespace mld::internal {
template <std::unsigned_integral WordT, typename Manager, u32 log>
struct binary_trie {
    static_assert(log > 0);

    using word_type = WordT;

    struct node;
    using handler = Manager::template handler<node>;
    using pointer = typename handler::pointer;

    using size_type = u32;
    using allocator_type = typename handler::allocator_type;

protected:
    [[no_unique_address]] handler hlr;

    static constexpr size_type arity = 1 << log;
    static constexpr size_type amask = arity - 1;

    static constexpr size_type word_size = std::numeric_limits<word_type>::digits;

    [[nodiscard]] static constexpr word_type mask(size_type l, size_type r) noexcept {
        word_type u = r < word_size ? (word_type{1} << r) : 0;
        word_type v = l < word_size ? (word_type{1} << l) : 0;
        return u - v;
    }

    [[nodiscard]] static constexpr word_type masked(word_type v, size_type l,
                                                    size_type r) noexcept {
        return mask(l, r) & v;
    }

    [[nodiscard]] static constexpr word_type diff_bit(word_type x,
                                                      word_type y) noexcept {
        if constexpr (std::has_single_bit(log))
            return (((word_size - 1 - std::countl_zero(x ^ y))) | (log - 1)) + 1;

        if (x == y) return 0;
        return ((word_size - 1 - std::countl_zero(x ^ y)) / log + 1) * log;
    }

public:
    explicit constexpr binary_trie(
        const allocator_type &alloc = allocator_type()) noexcept
        : hlr(alloc) {}

    struct node {
        std::array<pointer, arity> chi;
        word_type val = 0;
        size_type cnt = 0, wid = word_size;

        constexpr node() noexcept {
            chi.fill(handler::nil);
        }

        constexpr node(word_type x, size_type w) noexcept
            : val(x), cnt(1), wid(w) {
            chi.fill(handler::nil);
        }
    };

    constexpr void dispose(const pointer &v) noexcept {
        if (hlr.is_disposable(v)) {
            for (auto &&chi : v->chi) dispose(chi);
            hlr.dispose(v);
        }
    }

    constexpr void insert(pointer &t, word_type v, word_type z) noexcept {
        v ^= z;

        pointer r = t = hlr.clone(t);
        for (size_type bit = word_size;;) {
            word_type x = masked(v, bit - r->wid, bit),
                      y = masked(r->val, bit - r->wid, bit);
            if (x != y) {
                size_type d = diff_bit(x, y);
                size_type a = static_cast<size_type>((x >> (d - log)) & amask),
                          b = static_cast<size_type>((y >> (d - log)) & amask);

                pointer s = hlr.create(*r);
                s->wid -= bit - d;

                r->chi.fill(handler::nil);
                r->chi[a] = hlr.create(v, d);
                r->chi[b] = s;

                ++r->cnt;
                r->wid = bit - d;
                return;
            }

            ++r->cnt;
            if (bit -= r->wid; bit == 0) return;

            pointer &s = r->chi[(v >> (bit - log)) & amask];
            if (s == handler::nil) {
                s = hlr.create(v, bit);
                return;
            }

            r = s = hlr.clone(s);
        }
    }

    [[nodiscard]] static constexpr size_type count(const pointer &t, word_type v,
                                                   word_type z) noexcept {
        v ^= z;

        pointer r = t;
        for (size_type bit = word_size; r != handler::nil;
             r = r->chi[(v >> (bit - log)) & amask]) {
            word_type x = masked(v, bit - r->wid, bit),
                      y = masked(r->val, bit - r->wid, bit);
            if (x != y) return 0;
            if (bit -= r->wid; bit == 0) return r->cnt;
        }

        return 0;
    }

    constexpr void erase(pointer &t, word_type v, word_type z, size_type n) noexcept {
        v ^= z;

        pointer r = t = hlr.clone(t);
        for (size_type bit = word_size;;) {
            r->cnt -= n;
            if (bit -= r->wid; bit == 0) return;

            pointer &s = r->chi[(v >> (bit - log)) & amask];
            r = s = hlr.clone(s);
        }
    }

    [[nodiscard]] constexpr word_type kth(const pointer &t, size_type k,
                                          word_type z) const noexcept {
        ++k;
        pointer r = t;
        for (size_type bit = word_size;;) {
            if (bit -= r->wid; bit == 0) return r->val ^ z;

            size_type b = static_cast<size_type>((z >> (bit - log)) & amask);
            auto it = std::ranges::find_if(std::views::iota(size_type{0}, arity),
                                           [&](auto d) {
                                               pointer c = r->chi[b ^ d];
                                               if (k <= c->cnt) return true;
                                               k -= c->cnt;
                                               return false;
                                           });

            r = r->chi[b ^ *it];
        }
    }

    [[nodiscard]] constexpr word_type min(const pointer &t,
                                          word_type z) const noexcept {
        pointer r = t;
        for (size_type bit = word_size;;) {
            if (bit -= r->wid; bit == 0) return r->val ^ z;

            size_type b = static_cast<size_type>((z >> (bit - log)) & amask);
            auto it = std::ranges::find_if(std::views::iota(size_type{0}, arity),
                                           [&](auto d) { return r->chi[b ^ d]->cnt; });
            r = r->chi[b ^ *it];
        }
    }

    [[nodiscard]] constexpr word_type max(const pointer &t,
                                          word_type z) const noexcept {
        pointer r = t;
        for (size_type bit = word_size;;) {
            if (bit -= r->wid; bit == 0) return r->val ^ z;

            size_type b = static_cast<size_type>((z >> (bit - log)) & amask);
            auto it = std::ranges::find_if(std::views::iota(size_type{0}, arity) |
                                               std::views::reverse,
                                           [&](auto d) { return r->chi[b ^ d]->cnt; });
            r = r->chi[b ^ *it];
        }
    }

    [[nodiscard]] constexpr size_type rank(const pointer &t, word_type v,
                                           word_type z) const noexcept {
        size_type rnk = 0;
        if (v == 0) return rnk;

        --v;
        pointer r = t;
        for (size_type bit = word_size; r != handler::nil;) {
            word_type x = masked(v, bit - r->wid, bit),
                      y = masked(r->val ^ z, bit - r->wid, bit);
            if (x < y) return rnk;
            if (x > y) return rnk + r->cnt;
            if (bit -= r->wid; bit == 0) return rnk + r->cnt;

            size_type b = static_cast<size_type>((v >> (bit - log)) & amask),
                      c = static_cast<size_type>((z >> (bit - log)) & amask);

            auto u = std::views::iota(size_type{0}, b) |
                     std::views::transform([&](auto d) { return r->chi[c ^ d]->cnt; });
            rnk += std::reduce(u.begin(), u.end());

            r = r->chi[b ^ c];
        }

        return rnk;
    }
};
} // namespace mld::internal

#endif // MALLARD_INTERNAL_BINARY_TRIE_H
