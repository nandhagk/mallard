#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/lxor.h"
#include "lib/compress.h"
#include "lib/hash.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/wavelet_matrix_aggregate.h"

void solve() {
    u32 n;
    std::cin >> n;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    mld::hash<u64> h;
    mld::wavelet_matrix_aggregate<mld::scan_table<mld::algebra::lxor<u64>>> wma(
        std::views::iota(u32{0}, n) | std::views::transform([&](u32 k) {
            return std::make_pair(na[k], h(na[k]));
        }),
        static_cast<u32>(v.size()) + 1);

    u32 q;
    std::cin >> q;

    for (u32 ans = 0; q--;) {
        u32 l, r;
        std::cin >> l >> r;

        l ^= ans, r ^= ans;
        --l;

        u32 lo = 0, hi = static_cast<u32>(v.size()) + 1;
        while (hi - lo > 1) {
            u32 m = std::midpoint(lo, hi);
            auto [_, y] = wma.rank(l, r, m);
            if (y != 0)
                hi = m;
            else
                lo = m;
        }

        ans = lo == static_cast<u32>(v.size()) ? 0 : v[lo];
        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
