#ifndef MALLARD_RADIX_HEAP_H
#define MALLARD_RADIX_HEAP_H 1

#include <array>
#include <cassert>
#include <concepts>
#include <limits>
#include <type_traits>
#include <vector>

#include "lib/internal/dummy.h"
#include "lib/prelude.h"

namespace mld {
// WARNING: Can only use if keys are monotonic
template <std::unsigned_integral WordT, typename V>
struct radix_heap {
    using word_type = WordT;
    using size_type = u32;
    using value_type = V;

private:
    static constexpr size_type word_size = std::numeric_limits<word_type>::digits;
    static constexpr word_type inf = std::numeric_limits<word_type>::max();

    std::array<std::vector<std::pair<word_type, value_type>>, word_size + 1> buf;
    std::array<word_type, word_size + 1> min;
    word_type flg, lst;
    size_type len;

    [[gnu::always_inline, nodiscard]] constexpr size_type
    pos(word_type a) const noexcept {
        return word_size - std::countl_zero(a);
    }

    constexpr void insert(word_type key, const value_type &val) noexcept {
        size_type b = pos(key ^ lst);
        buf[b].emplace_back(key, val);
        min[b] = std::min(key, min[b]);
        flg |= word_type{1} << b;
    }

    constexpr void pull() noexcept {
        if (flg & word_type{1}) return;

        size_type idx = std::countr_zero(flg);
        lst = min[idx];

        for (auto &&[key, val] : buf[idx]) insert(key, val);

        buf[idx].clear();
        min[idx] = inf;
        flg &= ~(word_type{1} << idx);
    }

public:
    constexpr radix_heap() noexcept
        : flg(0), lst(0), len(0) {
        min.fill(inf);
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return len == 0;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    constexpr void clear() noexcept {
        for (auto &&b : buf) b.clear();
        min.fill(inf);

        flg = lst = 0;
        len = 0;
    }

    constexpr void emplace(word_type key, const value_type &val) noexcept {
        assert(lst <= key);
        ++len, insert(key, val);
    }

    [[nodiscard]] constexpr auto top() noexcept {
        pull();
        return buf[0].back();
    }

    constexpr void pop() noexcept {
        pull();
        if (--len, buf[0].pop_back(); buf[0].empty()) {
            min[0] = inf;
            flg &= ~word_type{1};
        }
    }
};

template <std::unsigned_integral WordT>
struct radix_heap<WordT, void> {
    using word_type = WordT;
    using size_type = u32;

private:
    static constexpr size_type word_size = std::numeric_limits<word_type>::digits;
    static constexpr word_type inf = std::numeric_limits<word_type>::max();

    std::array<std::vector<word_type>, word_size + 1> buf;
    std::array<word_type, word_size + 1> min;
    word_type flg, lst;
    size_type len;

    [[gnu::always_inline, nodiscard]] constexpr size_type
    pos(word_type a) const noexcept {
        return word_size - std::countl_zero(a);
    }

    constexpr void insert(word_type key) noexcept {
        size_type b = pos(key ^ lst);
        buf[b].push_back(key);
        min[b] = std::min(key, min[b]);
        flg |= word_type{1} << b;
    }

    constexpr void pull() noexcept {
        if (flg & word_type{1}) return;

        size_type idx = std::countr_zero(flg);
        lst = min[idx];

        for (word_type key : buf[idx]) insert(key);

        buf[idx].clear();
        min[idx] = inf;
        flg &= ~(word_type{1} << idx);
    }

public:
    constexpr radix_heap() noexcept
        : flg(0), lst(0), len(0) {
        min.fill(inf);
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return len == 0;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    constexpr void clear() noexcept {
        for (auto &&b : buf) b.clear();
        min.fill(inf);

        flg = lst = 0;
        len = 0;
    }

    constexpr void emplace(word_type key) noexcept {
        assert(lst <= key);
        ++len, insert(key);
    }

    [[nodiscard]] constexpr auto top() noexcept {
        pull();
        return buf[0].back();
    }

    constexpr void pop() noexcept {
        pull();
        if (--len, buf[0].pop_back(); buf[0].empty()) {
            min[0] = inf;
            flg &= ~word_type{1};
        }
    }
};
} // namespace mld

#endif // MALLARD_RADIX_HEAP_H
