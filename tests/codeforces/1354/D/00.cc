#include <bits/stdc++.h>

#include "lib/segment_tree.h"
#include "lib/algebra/sum.h"
#include "lib/prelude.h"

static constexpr u32 N = 1'000'000;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> cnt(N);
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        ++cnt[--a];
    }

    mld::segment_tree<mld::algebra::sum<u32>> st(cnt);
    while (q--) {
        i32 k;
        std::cin >> k;

        if (k > 0) {
            st.add(--k, 1);
        } else {
            k = -k;
            u32 r = st.max_right(0, [k](auto &&x) { return x < k; });
            assert(r < N);
            st.add(r, -1);
        }
    }

    u32 z = st.max_right(0, [](auto &&x) { return x == 0; });
    std::cout << (z == N ? 0 : z + 1) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
