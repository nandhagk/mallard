#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    for (k = n - k; k;) {
        u32 z = std::bit_width(k) - 1;
        u32 x = 1 << z;
        u32 w = a.size() - x;

        for (u32 i = 0; i < w; ++i) a[i] = a[i] ^ a[i + x];
        k ^= x;
        n -= x;
    }

    for (auto &&x : a | std::views::take(n)) std::cout << x << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
