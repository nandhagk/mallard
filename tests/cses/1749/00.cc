#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::segment_tree<mld::algebra::sum<u32>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 1; }));

    for (u32 i = 0; i < n; ++i) {
        u32 p;
        std::cin >> p;

        --p;
        u32 k = st.max_right(0, [&](auto &&x) { return x <= p; });

        st.set(k, 0);
        std::cout << a[k] << ' ';
    }

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
