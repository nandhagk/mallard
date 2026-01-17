#include <bits/stdc++.h>

#include "lib/actions/base.h"
#include "lib/algebra/base.h"
#include "lib/algebra/lxor.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

struct lucky : mld::algebra::base<std::tuple<u32, u32, u32, u32>>,
               mld::algebra::associative {
    using operand = mld::algebra::base<std::tuple<u32, u32, u32, u32>>;

    using operand::operand;
    using operand::val;

    lucky(char c) noexcept
        : lucky(c == '4', c == '7', 1, 1) {}

    [[nodiscard]] friend lucky operator+(const lucky &lhs, const lucky &rhs) noexcept {
        auto &&[a, b, c, d] = lhs.val();
        auto &&[p, q, r, s] = rhs.val();
        return {a + p, b + q, std::max({c + q, a + r, a + q}),
                std::max({d + p, b + s, b + p})};
    }
};

struct action : mld::actions::base<mld::algebra::lxor<u32>>::identity {
    using operand = lucky;
    using operation = mld::algebra::lxor<u32>;

    [[nodiscard]] static operand act(const operation &f, const operand &x) noexcept {
        auto [a, b, c, d] = x.val();
        if (f) std::swap(a, b), std::swap(c, d);
        return {a, b, c, d};
    }
};

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::string s;
    std::cin >> s;

    mld::lazy_segment_tree<action> st(s);

    while (m--) {
        std::string t;
        std::cin >> t;

        if (t == "switch") {
            u32 l, r;
            std::cin >> l >> r;

            st.apply(--l, r, 1);
        } else {
            std::cout << std::get<2>(st.fold().val()) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
