#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/mxss.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::mxss<i64>;
    mld::segment_tree<operand> st(std::views::iota(u32{0}, n) |
                                  std::views::transform([](auto) {
                                      i32 a;
                                      std::cin >> a;
                                      return operand(a);
                                  }));

    while (q--) {
        u32 a, b;
        std::cin >> a >> b;

        std::cout << static_cast<u64>(
                         std::max<i64>(0, std::get<3>(st.fold(--a, b).val())))
                  << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
