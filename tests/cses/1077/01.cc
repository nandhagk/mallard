#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/foldable_kth_heap.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::scan_table<mld::algebra::sum<u64>> b(a);
    mld::foldable_kth_heap<u32, mld::algebra::sum<u64>, std::greater<>> fkh((k - 1) /
                                                                            2);

    for (u32 i = 0; i < k - 1; ++i) fkh.insert(a[i], a[i]);
    for (u32 i = k - 1; i < n; ++i) {
        fkh.insert(a[i], a[i]);

        auto [v, x] = fkh.kth();
        std::cout << b.fold(i - k + 1, i + 1).val() - 2 * x.val() - (2 - (k % 2)) * v << ' ';

        fkh.erase(a[i - k + 1], a[i - k + 1]);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
