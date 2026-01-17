#ifndef MALLARD_FENWICK_SET_H
#define MALLARD_FENWICK_SET_H 1

#include <bit>
#include <concepts>
#include <limits>

#include "lib/algebra/sum.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"

namespace mld {
template <std::unsigned_integral WordT = u64>
struct fenwick_set {
    using word_type = WordT;
    using size_type = u32;

private:
    static constexpr size_type word_size = std::numeric_limits<word_type>::digits;

    size_type len, cap;
    std::vector<word_type> buf;
    fenwick_tree<algebra::sum<size_type>> fwt;

public:
    constexpr fenwick_set() noexcept = default;

    explicit constexpr fenwick_set(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n, cap = (len + word_size - 1) / word_size;

        buf.assign(cap, word_type{0});
        fwt.build(cap);
    }

    [[nodiscard]] constexpr size_type rank(size_type k) const noexcept {
        assert(k <= len);

        word_type msk = (word_type{1} << (k % word_size)) - 1;
        return fwt.fold(k / word_size).val() + std::popcount(buf[k / word_size] & msk);
    }

    [[nodiscard]] constexpr size_type rank(size_type l, size_type r) const noexcept {
        assert(l < r && r <= len);
        if (l == 0) return rank(r);

        word_type lmsk = (word_type{1} << (l % word_size)) - 1;
        word_type rmsk = (word_type{1} << (r % word_size)) - 1;
        return fwt.fold(l / word_size, r / word_size).val() +
               std::popcount(buf[r / word_size] & rmsk) -
               std::popcount(buf[l / word_size] & lmsk);
    }

    constexpr void insert(size_type k) noexcept {
        buf[k / word_size] |= word_type{1} << (k % word_size);
        fwt.add(k / word_size, 1);
    }

    constexpr void erase(size_type k) noexcept {
        buf[k / word_size] &= ~(word_type{1} << (k % word_size));
        fwt.add(k / word_size, -1);
    }
};
} // namespace mld

#endif // MALLARD_FENWICK_SET_H
