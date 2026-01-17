#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/compress.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/wavelet_matrix_aggregate.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    auto &&[v, na] = mld::compress(a);
    mld::wavelet_matrix_aggregate<mld::scan_table<mld::algebra::sum<u64>>> wma(
        std::views::iota(u32{0}, n) |
            std::views::transform([&](u32 i) { return std::make_pair(na[i], a[i]); }),
        static_cast<u32>(v.size()));

    while (q--) {
        u32 l, r, x;
        std::cin >> l >> r >> x;

        if (l == r) {
            std::cout << 0 << ' ' << 0 << '\n';
            continue;
        }

        u32 p = static_cast<u32>(std::upper_bound(v.begin(), v.end(), x) - v.begin());
        auto &&[y, z] = wma.rank(l, r, p);
        std::cout << y << ' ' << z.val() << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
