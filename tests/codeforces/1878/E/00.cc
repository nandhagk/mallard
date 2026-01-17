#include <bits/stdc++.h>

#include "lib/algebra/land.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::segment_tree<mld::algebra::land<u32>> st(std::views::iota(u32{0}, n) |
                                                  std::views::transform([](auto) {
                                                      u32 a;
                                                      std::cin >> a;
                                                      return a;
                                                  }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 l, k;
        std::cin >> l >> k;

        u32 r = st.max_right(--l, [&](auto &&x) { return x >= k; });
        std::cout << i32(l == r ? -1 : r) << ' ';
    }

    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
