#ifndef MALLARD_HASH_HPP
#define MALLARD_HASH_HPP 1

#include <concepts>
#include <type_traits>

#include "lib/prelude.h"
#include "lib/rand.h"

namespace mld {
template <typename T>
struct hash {};

template <typename T>
constexpr void hash_combine(u64 &seed, const T &v) noexcept {
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4);
};

template <std::integral T>
struct hash<T> {
    [[nodiscard]] u64 operator()(T y) const noexcept {
        u64 x = y;
        x += 0x9e3779b97f4a7c15 + SEED;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
};

template <std::ranges::range T>
struct hash<T> {
    [[nodiscard]] u64 operator()(const T &a) const noexcept {
        u64 value = SEED;
        for (auto &&x : a) hash_combine(value, x);
        return value;
    }
};

template <typename... T>
struct hash<std::tuple<T...>> {
    [[nodiscard]] u64 operator()(const std::tuple<T...> &a) const noexcept {
        u64 value = SEED;
        std::apply([&value](T &&...args) { (hash_combine(value, args), ...); }, a);
        return value;
    }
};

template <typename T, typename U>
struct hash<std::pair<T, U>> {
    [[nodiscard]] u64 operator()(const std::pair<T, U> &a) const noexcept {
        u64 value = SEED;
        hash_combine(value, a.first);
        hash_combine(value, a.second);
        return value;
    }
};
} // namespace mld

#endif // MALLARD_HASH_HPP
