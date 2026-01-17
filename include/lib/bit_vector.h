#ifndef MALLARD_BIT_VECTOR_H
#define MALLARD_BIT_VECTOR_H 1

#include <array>
#include <bit>
#include <concepts>
#include <limits>
#include <vector>

#include "lib/prelude.h"

namespace mld {

template <std::unsigned_integral WordT>
struct bit_vector {
    using word_type = WordT;
    using size_type = u32;

private:
    static constexpr size_type word_size = std::numeric_limits<word_type>::digits;

    size_type len;
    std::vector<word_type> bit;
    std::vector<size_type> sum;

public:
    size_type zeros;

    constexpr explicit bit_vector(size_type n) noexcept
        : len(n), bit(len / word_size + 1), sum(len / word_size + 1) {}

    constexpr void build() noexcept {
        for (size_type i = 0; i < len / word_size; ++i)
            sum[i + 1] = sum[i] + static_cast<size_type>(std::popcount(bit[i]));

        zeros = rank0(len);
    }

    constexpr void set(size_type k) noexcept {
        bit[k / word_size] |= word_type{1} << (k % word_size);
    }

    [[nodiscard]] constexpr bool operator[](size_type k) const noexcept {
        return (bit[k / word_size] >> (k % word_size)) & 1;
    }

    [[nodiscard]] constexpr size_type rank1(size_type r) const noexcept {
        word_type mask = (word_type{1} << (r % word_size)) - 1;
        return sum[r / word_size] + std::popcount(bit[r / word_size] & mask);
    }

    [[nodiscard]] constexpr size_type rank0(size_type r) const noexcept {
        return r - rank1(r);
    }

    [[nodiscard]] constexpr size_type rank1(size_type l, size_type r) const noexcept {
        return rank1(r) - rank1(l);
    }

    [[nodiscard]] constexpr size_type rank0(size_type l, size_type r) const noexcept {
        return rank0(r) - rank0(l);
    }
};
} // namespace mld

#endif // MALLARD_BIT_VECTOR_H
