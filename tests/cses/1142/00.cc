#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::segment_tree<mld::algebra::min<u32>> st(std::views::iota(u32{0}, n) |
                                                 std::views::transform([](auto) {
                                                     u32 a;
                                                     std::cin >> a;
                                                     return a;
                                                 }));

    u64 m = 0;
    for (u32 i = 0; i < n; ++i) {
        u32 k = st.get(i).val();

        u32 l = i == 0 ? 0 : st.min_left(i, [k](auto &&x) { return x >= k; });
        u32 r = st.max_right(i, [k](auto &&x) { return x >= k; });

        m = std::max(m, static_cast<u64>(r - l) * k);
    }

    std::cout << m << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
