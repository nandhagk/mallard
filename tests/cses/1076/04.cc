#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/kth_heap.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::kth_heap<u32, std::greater<>> kh((k - 1) / 2);
    for (u32 i = 0; i < k - 1; ++i) kh.insert(a[i]);

    for (u32 i = k - 1; i < n; ++i) {
        kh.insert(a[i]);
        std::cout << kh.kth() << ' ';
        kh.erase(a[i - k + 1]);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
