#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    u32 k = std::bit_ceil(n);
    u32 z = std::bit_width(k) - 1;

    std::vector<u32> a(k);
    for (auto &&x : a | std::views::take(n)) std::cin >> x;

    for (u32 i = 0; i < z; ++i) {
        for (u32 j = 0; j < k; ++j)
            if ((j >> i) & 1) a[j] ^= a[j ^ (1 << i)];
    }

    for (auto &&x : a | std::views::take(n)) std::cout << x << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
