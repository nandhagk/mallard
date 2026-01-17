#include <bits/stdc++.h>

#include "lib/actions/base.h"
#include "lib/algebra/base.h"
#include "lib/lazy_segment_tree.h"

template <std::unsigned_integral T, usize K>
struct sum : mld::algebra::base<std::array<T, K>>,
             mld::algebra::associative,
             mld::algebra::commutative<sum<T, K>> {
    using operand = mld::algebra::base<std::array<T, K>>;

    using operand::operand;
    using operand::val;

    explicit sum(T t) noexcept
        : sum(([t]() {
              std::array<T, K> x{};
              for (u32 i = 0; i < K; ++i) x[i] = (t >> i) & 1;
              return x;
          })()) {}

    [[nodiscard]] friend sum operator+(const sum &lhs, const sum &rhs) noexcept {
        std::array<T, K> x{};
        std::ranges::transform(lhs.val(), rhs.val(), x.begin(), std::plus{});
        return x;
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] friend sum operator*(const sum &lhs, Scalar n) noexcept {
        std::array<T, K> x{};
        std::ranges::transform(lhs.val(), x.begin(), [n](u32 y) { return y * n; });
        return x;
    }
};

template <std::unsigned_integral T, usize K>
struct lxor : mld::algebra::base<std::array<T, K>>,
              mld::algebra::associative,
              mld::algebra::commutative<lxor<T, K>>,
              mld::algebra::truthy<lxor<T, K>> {
    using operand = mld::algebra::base<std::array<T, K>>;

    using operand::operand;
    using operand::val;

    explicit lxor(T t) noexcept
        : lxor(([t]() {
              std::array<T, K> x{};
              for (u32 i = 0; i < K; ++i) x[i] = (t >> i) & 1;
              return x;
          })()) {}

    [[nodiscard]] friend lxor operator+(const lxor &lhs, const lxor &rhs) noexcept {
        std::array<T, K> x{};
        std::ranges::transform(lhs.val(), rhs.val(), x.begin(), std::bit_xor{});
        return x;
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] friend lxor operator*(const lxor &lhs, Scalar n) noexcept {
        std::array<T, K> x{};
        std::ranges::transform(lhs.val(), x.begin(), [n](u32 y) { return y * n; });
        return x;
    }
};

struct action : mld::actions::base<lxor<u32, 20>>::automatic {
    using operand = sum<u32, 20>;
    using operation = lxor<u32, 20>;

    [[nodiscard]] static operand act(const operation &f, const operand &x) noexcept {
        std::array<u32, 20> y{};
        std::ranges::transform(x.val(), f.val(), y.begin(),
                               [](u32 a, u32 b) { return b ? b - a : a; });
        return y;
    }
};

void solve() {
    u32 n;
    std::cin >> n;

    mld::lazy_segment_tree<action> st(std::views::iota(u32{0}, n) |
                                      std::views::transform([](auto) {
                                          u32 a;
                                          std::cin >> a;
                                          return sum<u32, 20>(a);
                                      }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 l, r;
            std::cin >> l >> r;

            auto z = st.fold(--l, r).val();

            u64 ans = 0;
            for (u32 i = 0; i < 20; ++i) ans += (u64(1) << i) * z[i];

            std::cout << ans << '\n';
        } else {
            u32 l, r, x;
            std::cin >> l >> r >> x;

            st.apply(--l, r, lxor<u32, 20>(x));
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
