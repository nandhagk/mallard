#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/max.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::segment_tree<mld::algebra::max<u32>> st(std::views::iota(u32{0}, n) |
                                                 std::views::transform([](auto) {
                                                     u32 a;
                                                     std::cin >> a;
                                                     return a;
                                                 }));

    while (q--) {
        u32 r;
        std::cin >> r;

        u32 k = st.max_right(0, [&](auto &&h) { return h < r; });
        if (k == n) {
            std::cout << 0 << ' ';
            continue;
        }

        st.set(k, st.get(k).val() - r);
        std::cout << k + 1 << ' ';
    }

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
