#include <bits/stdc++.h>

#include "lib/algebra/max.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<u32> a(n * m);
    for (u32 i = 0; i < n; ++i)
        for (u32 j = 0; j < m; ++j) std::cin >> a[j * n + i];

    mld::four_russian<mld::sparse_table<mld::algebra::max<u32>>> st(a);

    std::vector<std::tuple<u32, u32, u32>> x;
    x.reserve(n);
    for (u32 l = 0; l < n; ++l) {
        u32 lo = l, hi = n + 1;
        while (hi - lo > 1) {
            u32 r = std::midpoint(lo, hi);
            u32 z = 0;
            for (u32 j = 0; j < m; ++j) z += st.fold(j * n + l, j * n + r).val();

            if (z > k) {
                hi = r;
            } else {
                lo = r;
            }
        }

        x.emplace_back(lo - l, l, lo);
    }

    auto &&[_, l, r] = *std::ranges::max_element(x);
    for (u32 i = 0; i < m; ++i) std::cout << (l == r ? 0 : st.fold(i * n + l, i * n + r).val()) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
