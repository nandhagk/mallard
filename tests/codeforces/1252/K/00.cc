#include <bits/stdc++.h>

#include "lib/actions/base.h"
#include "lib/algebra/base.h"
#include "lib/algebra/lxor.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint1000000007;

template <typename T>
struct robot : mld::algebra::base<std::tuple<T, T, T, T>>, mld::algebra::associative {
    using operand = mld::algebra::base<std::tuple<T, T, T, T>>;

    using operand::operand;
    using operand::val;

    robot() noexcept
        : robot(1, 0, 0, 1) {}

    robot(char c) noexcept
        : robot(1, c == 'A', c == 'B', 1) {}

    [[nodiscard]] friend robot operator+(const robot &lhs, const robot &rhs) noexcept {
        auto &&[a, b, c, d] = lhs.val();
        auto &&[p, q, r, s] = rhs.val();
        return {p * a + q * c, p * b + q * d, r * a + s * c, r * b + s * d};
    }
};

struct action : mld::actions::base<mld::algebra::lxor<bool>>::identity {
    using operand = robot<Z>;
    using operation = mld::algebra::lxor<bool>;

    [[nodiscard]] static operand act(const operation &f, const operand &x) noexcept {
        auto &&[a, b, c, d] = x.val();
        if (f.val()) std::swap(a, d), std::swap(b, c);
        return {a, b, c, d};
    }
};

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::string s;
    std::cin >> s;

    mld::lazy_segment_tree<action> st(s);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 l, r;
            std::cin >> l >> r;

            st.apply(--l, r, 1);
        } else {
            u32 l, r, x, y;
            std::cin >> l >> r >> x >> y;

            auto &&[a, b, c, d] = st.fold(--l, r).val();
            std::cout << (a * x + b * y).val() << ' ' << (c * x + d * y).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
