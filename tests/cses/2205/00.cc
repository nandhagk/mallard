#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    for (u32 i = 0, c = 0, z = 1 << n; i < z; ++i) {
        std::cout << std::format("{:0{}b}\n", c, n);
        c ^= (1 << std::countr_zero(i + 1));
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
