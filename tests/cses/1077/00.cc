#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/compress.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/wavelet_matrix_aggregate.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::scan_table<mld::algebra::sum<u64>> b(a);
    auto [v, na] = mld::compress(a);

    mld::wavelet_matrix_aggregate<mld::scan_table<mld::algebra::sum<u64>>> wm(
        std::views::iota(u32{0}, n) |
            std::views::transform([&](u32 i) { return std::make_pair(na[i], a[i]); }),
        static_cast<u32>(v.size()));

    for (u32 i = 0; i <= n - k; ++i) {
        auto [m, l] = wm.kth(i, i + k, k / 2);

        u64 lo = u64(v[m]) * (k / 2) - l.val();
        u64 hi = b.fold(i, i + k).val() - l.val() - u64(v[m]) * ((k + 1) / 2);

        std::cout << lo + hi << ' ';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
