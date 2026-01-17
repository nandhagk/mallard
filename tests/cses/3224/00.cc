#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/mode_heap.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::mode_heap<u32, std::greater<u32>> mh;
    mh.reserve(n);

    for (u32 i = 0; i < k - 1; ++i) mh.insert(a[i]);
    for (u32 i = k - 1; i < n; ++i) {
        mh.insert(a[i]);
        std::cout << mh.mode().first << ' ';
        mh.erase(a[i - k + 1]);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
