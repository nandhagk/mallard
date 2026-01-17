#ifndef MALLARD_BARRETT_H
#define MALLARD_BARRETT_H 1

#include <concepts>

#include "lib/prelude.h"

namespace mld {
struct barrett_32 {
    constexpr barrett_32(u32 mod) noexcept
        : m(mod), im(u64(-1) / m + 1) {}

    constexpr u32 mod() const noexcept {
        return m;
    }

    constexpr u32 modulo(u64 z) const noexcept {
        if (m == 1) return 0;

        u64 x = u64((u128(z) * im) >> 64);
        u64 y = x * m;

        return u32(z - y + (z < y ? m : 0));
    }

    constexpr u32 mul(u32 a, u32 b) const noexcept {
        return modulo(u64(a) * b);
    }

private:
    u32 m;
    u64 im;
};

struct barrett_64 {
    constexpr barrett_64(u64 mod) noexcept
        : m(mod) {
        u128 im = u128(-1) / m;
        if (im * m + m == u128(0)) ++im;

        mh = im >> 64;
        ml = im & u64(-1);
    }

    constexpr u64 mod() const noexcept {
        return m;
    }

    constexpr u64 modulo(u128 x) const noexcept {
        u128 z = (x & u64(-1)) * ml;

        z = (x & u64(-1)) * mh + (x >> 64) * ml + (z >> 64);
        z = (x >> 64) * mh + (z >> 64);

        x -= z * m;
        return u64(x < m ? x : x - m);
    }

    constexpr u64 mul(u64 a, u64 b) const noexcept {
        return modulo(u128(a) * b);
    }

private:
    u64 m;
    u128 mh{}, ml{};
};

template <std::unsigned_integral U>
struct barrett {
    using type = barrett_32;
};

template <>
struct barrett<u64> {
    using type = barrett_64;
};

template <std::unsigned_integral U>
using barrett_t = barrett<U>::type;
} // namespace mld

#endif // MALLARD_BARRETT_H
