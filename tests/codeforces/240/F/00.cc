#include <bits/stdc++.h>

#include "lib/actions/assign.h"
#include "lib/algebra/base.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

template <typename T, usize K>
struct substr : mld::algebra::base<std::array<T, K>>,
                mld::algebra::associative,
                mld::algebra::commutative<substr<T, K>> {
    using operand = mld::algebra::base<std::array<T, K>>;

    using operand::operand;
    using operand::val;

    explicit substr(u32 c) noexcept
        : substr(([c]() {
              std::array<T, K> x{};
              ++x[c];
              return x;
          })()) {}

    [[nodiscard]] friend substr operator+(const substr &lhs,
                                          const substr &rhs) noexcept {
        std::array<T, K> x{};
        std::ranges::transform(lhs.val(), rhs.val(), x.begin(), std::plus<T>{});
        return x;
    }

    template <std::unsigned_integral Scalar>
    [[nodiscard]] friend substr operator*(const substr &lhs, Scalar n) noexcept {
        std::array<T, K> x{};
        std::ranges::transform(lhs.val(), x.begin(), [n](u32 y) { return y * n; });
        return x;
    }
};

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::string s;
    std::cin >> s;

    mld::lazy_segment_tree<mld::actions::assign<substr<u32, 26>>> st(
        s | std::views::transform([](char c) { return substr<u32, 26>(c - 'a'); }));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        auto z = st.fold(--l, r).val();
        if (std::ranges::count_if(z, [](u32 c) { return c % 2; }) > 1) continue;

        u32 a = static_cast<u32>(std::ranges::find_if(z, [](u32 c) { return c % 2; }) -
                                 z.begin());
        for (u32 i = 0; i < 26; l += z[i] / 2, r -= z[i] / 2, ++i) {
            if (z[i] < 2) continue;

            st.apply(l, l + z[i] / 2, substr<u32, 26>(i));
            st.apply(r - z[i] / 2, r, substr<u32, 26>(i));
        }

        if (a != 26) {
            assert(l + 1 == r);
            st.set(l, substr<u32, 26>(a));
        } else {
            assert((r - l) % 2 == 0);
        }
    }

    for (u32 i = 0; i < n; ++i) {
        auto z = st.get(i).val();
        for (u32 j = 0; j < 26; ++j)
            if (z[j]) std::cout << static_cast<char>('a' + j);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

#ifndef NANDHAGK_LOCAL
    std::ifstream in("input.txt");
    std::cin.rdbuf(in.rdbuf());

    std::ofstream out("output.txt");
    std::cout.rdbuf(out.rdbuf());
#endif

    solve();
}
