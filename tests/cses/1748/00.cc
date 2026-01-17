#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/compress.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"
#include "lib/wavelet_matrix_aggregate.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n;
    std::cin >> n;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    mld::wavelet_matrix_aggregate<mld::fenwick_tree<mld::algebra::sum<Z>>> wma(
        na | std::views::transform([](u32 k) { return std::make_pair(k, 0); }),
        static_cast<u32>(v.size()));

    Z ans = 0;
    for (u32 i = 0; i < n; ++i) {
        auto [_, x] = wma.rank(0, i + 1, na[i]);
        x = x + 1;
        ans += x.val();
        wma.add(i, x);
    }

    std::cout << ans.val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
