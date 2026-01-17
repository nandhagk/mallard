#include <bits/stdc++.h>

#include "lib/algebra/mxss.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    using operand = mld::algebra::mxss<i64>;
    mld::segment_tree<operand> sl(std::views::iota(u32{0}, n - 1) |
                                  std::views::transform([&](u32 i) {
                                      return operand(i64(a[i + 1]) - a[i] - 1);
                                  }));
    mld::segment_tree<operand> sr(std::views::iota(u32{0}, n - 1) |
                                  std::views::transform([&](u32 i) {
                                      return operand(i64(a[i]) - a[i + 1] - 1);
                                  }));

    std::cout << std::max({static_cast<i64>(0), std::get<3>(sl.fold().val()),
                           std::get<3>(sr.fold().val())})
              << '\n';
    while (q--) {
        u32 p, x;
        std::cin >> p >> x;

        --p;
        if (p > 0) {
            sl.set(p - 1, operand(i64(x) - a[p - 1] - 1));
            sr.set(p - 1, operand(i64(a[p - 1]) - x - 1));
        }

        if (p < n - 1) {
            sl.set(p, operand(i64(a[p + 1]) - x - 1));
            sr.set(p, operand(i64(x) - a[p + 1] - 1));
        }

        a[p] = x;
        std::cout << std::max({static_cast<i64>(0), std::get<3>(sl.fold().val()),
                               std::get<3>(sr.fold().val())})
                  << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
