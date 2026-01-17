#include <bits/stdc++.h>

#include "lib/algebra/base.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

struct factory : mld::algebra::base<std::pair<u64, u64>>, mld::algebra::associative {
    using operand = mld::algebra::base<std::pair<u64, u64>>;

    using operand::operand;
    using operand::val;

    friend factory operator+(const factory &lhs, const factory &rhs) noexcept {
        auto &&[a, b] = lhs.val();
        auto &&[p, q] = rhs.val();
        u64 x = std::min(a, q);
        return {a + p - x, b + q - x};
    }
};

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n), b(n);
    for (auto &&x : a) std::cin >> x;
    for (auto &&x : b) std::cin >> x;

    for (u32 i = 1; i < n; ++i) {
        u64 c;
        std::cin >> c;
    }

    u64 tot = std::reduce(b.begin(), b.end(), u64{0});
    mld::segment_tree<factory> st(
        std::views::iota(u32{0}, n) | std::views::transform([&](u32 k) {
            return a[k] > b[k] ? factory(a[k] - b[k], 0) : factory(0, b[k] - a[k]);
        }));

    while (q--) {
        u32 p, x, y;
        u64 z;
        std::cin >> p >> x >> y >> z;

        tot -= b[--p];
        tot += b[p] = y;
        a[p] = x;
        st.set(p, a[p] > b[p] ? factory(a[p] - b[p], 0) : factory(0, b[p] - a[p]));

        std::cout << tot - st.fold().val().second << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
