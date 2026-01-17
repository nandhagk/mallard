#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/base.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

template <typename T>
struct bit : mld::algebra::base<std::tuple<T, T, T, T, T, u32>>,
             mld::algebra::associative {
    using operand = mld::algebra::base<std::tuple<T, T, T, T, T, u32>>;

    using operand::operand;
    using operand::val;

    explicit bit(bool x) noexcept
        : bit({x, x, !x, !x, 1, 1}) {}

    [[nodiscard]] friend constexpr bit operator+(const bit &lhs,
                                                 const bit &rhs) noexcept {
        auto &&[a, b, c, d, e, f] = lhs.val();
        auto &&[u, v, w, x, y, z] = rhs.val();

        if (b == f) a = b + u;
        if (u == z) v = b + u;
        if (d == f) c = d + w;
        if (w == z) x = d + w;
        return {a, v, c, x, std::max({e, y, b + u, d + w}), f + z};
    }
};

void solve() {
    std::string s;
    std::cin >> s;

    using operand = bit<u32>;
    mld::segment_tree<operand> st(
        s | std::views::transform([](char c) { return operand(c == '0'); }));

    u32 m;
    std::cin >> m;

    while (m--) {
        u32 k;
        std::cin >> k;

        s[--k] ^= 1;
        st.set(k, operand(s[k] == '0'));

        std::cout << std::get<4>(st.fold().val()) << ' ';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
