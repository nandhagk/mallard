#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/base.h"
#include "lib/segment_tree.h"
#include "lib/prelude.h"

struct bracket : mld::algebra::base<std::tuple<u32, u32, u32>>,
                 mld::algebra::associative,
                 mld::algebra::truthy<bracket> {
    using operand = mld::algebra::base<std::tuple<u32, u32, u32>>;

    using operand::operand;
    using operand::val;

    bracket(char c) noexcept
        : bracket(0, c == '(', c == ')') {}

    [[nodiscard]] friend bracket operator+(const bracket &lhs,
                                           const bracket &rhs) noexcept {
        auto &&[a, b, c] = lhs.val();
        auto &&[p, q, r] = rhs.val();

        u32 z = std::min(b, r);
        return {a + p + z, b + q - z, c + r - z};
    }
};

void solve() {
    std::string s;
    std::cin >> s;

    mld::segment_tree<bracket> st(s);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        std::cout << std::get<0>(st.fold(--l, r).val()) * 2 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
