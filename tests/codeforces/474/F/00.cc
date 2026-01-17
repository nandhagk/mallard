#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/base.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

struct gcd : mld::algebra::base<std::pair<u32, u32>>,
             mld::algebra::associative,
             mld::algebra::commutative<gcd> {

    using operand = mld::algebra::base<std::pair<u32, u32>>;

    using operand::operand;
    using operand::val;

    gcd(u32 x) noexcept
        : gcd(x, 1) {}

    friend gcd operator+(const gcd &lhs, const gcd &rhs) {
        auto &&[a, b] = lhs.val();
        auto &&[p, q] = rhs.val();

        u32 g = std::gcd(a, p);
        return {g, b * (g == a) + q * (g == p)};
    }
};

void solve() {
    u32 n;
    std::cin >> n;

    mld::segment_tree<gcd> st(std::views::iota(u32{0}, n) |
                              std::views::transform([](auto) {
                                  u32 a;
                                  std::cin >> a;
                                  return a;
                              }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        --l;
        std::cout << (r - l - st.fold(l, r).val().second) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
