#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    using operand = mld::algebra::sum<u64>;
    mld::scan_table<operand> st(std::views::iota(u32{0}, n) |
                                  std::views::transform([](auto) {
                                      u32 a;
                                      std::cin >> a;
                                      return a;
                                  }));

    while (q--) {
        u32 a, b;
        std::cin >> a >> b;

        std::cout << st.fold(--a, b).val() << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
