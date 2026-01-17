#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/base.h"
#include "lib/potentialized_union_find.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

struct matrix_mul : mld::algebra::base<std::array<Z, 4>>, mld::algebra::associative {
    using value_type = std::array<Z, 4>;
    using operand = mld::algebra::base<value_type>;

    using operand::operand;
    using operand::val;

    matrix_mul() noexcept
        : matrix_mul(value_type{1, 0, 0, 1}) {}

    [[nodiscard]] friend matrix_mul
    operator+(const operand &a, const operand &b) noexcept {
        auto &&x = a.val();
        auto &&y = b.val();

        Z z00 = x[0] * y[0] + x[1] * y[2];
        Z z01 = x[0] * y[1] + x[1] * y[3];
        Z z10 = x[2] * y[0] + x[3] * y[2];
        Z z11 = x[2] * y[1] + x[3] * y[3];

        return value_type{z00, z01, z10, z11};
    }

    [[nodiscard]] matrix_mul operator-() noexcept {
        auto &&x = val();
        return value_type{x[3], -x[1], -x[2], x[0]};
    }
};

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::potentialized_union_find<matrix_mul> puf(n);
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 u, v, x00, x01, x10, x11;
            std::cin >> u >> v >> x00 >> x01 >> x10 >> x11;

            std::array<Z, 4> x{x00, x01, x10, x11};
            if (auto y = puf.potential(u, v)) {
                std::cout << (*y == x) << '\n';
            } else {
                std::cout << 1 << '\n';
                puf.merge(u, v, x);
            }
        } else {
            u32 u, v;
            std::cin >> u >> v;

            if (auto x = puf.potential(u, v)) {
                auto &&y = x->val();
                std::cout << y[0].val() << ' ' << y[1].val() << ' ' << y[2].val() << ' '
                          << y[3].val() << '\n';
            } else {
                std::cout << -1 << '\n';
            }
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
