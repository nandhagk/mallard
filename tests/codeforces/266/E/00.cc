#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/base.h"
#include "lib/algebra/assign.h"
#include "lib/algebra/combined.h"
#include "lib/algebra/sum.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

template <typename T>
struct action : mld::actions::base<mld::algebra::assign<T>>::identity {
    using operand = mld::algebra::combined<mld::algebra::sum<T>, mld::algebra::sum<T>>;
    using operation = mld::algebra::assign<T>;

    [[nodiscard]] static operand act(const operation &f, const operand &x) noexcept {
        if (!f) return x;

        auto &&[a, b] = x.val();
        return {a, a * (*f.val())};
    }
};

static constexpr u32 K = 5;
using Z = mld::modint1000000007;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<Z> a(n);
    for (auto &&x : a) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    std::array<mld::lazy_segment_tree<action<Z>>, K + 1> st{};
    for (u32 i = 0; i <= K; ++i) {
        st[i].build(std::views::iota(u32{0}, n) | std::views::transform([&](u32 k) {
                        Z w = Z(k + 1).pow(i);
                        return action<Z>::operand(w, w * a[k]);
                    }));
    }

    auto f = [&](u32 l, u32 r, u32 k) {
        std::array<Z, K + 1> x{};
        for (u32 i = 0; i <= K; ++i) x[i] = std::get<1>(st[i].fold(l, r).val());

        if (k == 0) return x[0];
        if (k == 1) return x[1] - l * (x[0]);
        if (k == 2) return x[2] - l * (x[1] * 2 - l * (x[0]));
        if (k == 3) return x[3] - l * (x[2] * 3 - l * (x[1] * 3 - l * (x[0])));
        if (k == 4)
            return x[4] - l * (x[3] * 4 - l * (x[2] * 6 - l * (x[1] * 4 - l * (x[0]))));
        if (k == 5) {
            return x[5] -
                   l * (x[4] * 5 - l * (x[3] * 10 -
                                        l * (x[2] * 10 - l * (x[1] * 5 - l * (x[0])))));
        }

        assert(0);
    };

    while (q--) {
        char t;
        std::cin >> t;

        if (t == '=') {
            u32 l, r, x;
            std::cin >> l >> r >> x;

            --l;
            for (u32 i = 0; i <= K; ++i) st[i].apply(l, r, x);
        } else {
            u32 l, r, k;
            std::cin >> l >> r >> k;

            std::cout << f(--l, r, k).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
