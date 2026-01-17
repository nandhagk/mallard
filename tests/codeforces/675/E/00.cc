#include <bits/stdc++.h>

#include "lib/algebra/min.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::segment_tree<mld::algebra::min<u64>> st(
        std::views::iota(u32{0}, n) |
        std::views::transform([](auto) { return std::numeric_limits<u64>::max(); }));

    std::vector<u32> a(n - 1);
    for (auto &&x : a) std::cin >> x;

    u64 ans = 0;
    st.set(n - 1, n);
    for (u32 i = n - 1; i--;) {
        u64 z = st.fold(i + 1, a[i]).val();
        u64 w = n + z - (a[i] + i + 1);
        ans += w;
        st.set(i, w + i + 1);
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
