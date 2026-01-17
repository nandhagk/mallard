#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    u32 u = 1 << n, v = 1 << m;
    std::vector<u32> a(u * v);
    for (u32 i = 0, c = 0; i < u; ++i) {
        for (u32 j = 0; j < v; ++j) {
            u32 k = i % 2 ? v - j - 1 : j;
            a[i * v + k] = c;
            c ^= (1 << std::countr_zero(i * v + j + 1));
        }
    }

    for (u32 i = 0; i < u; ++i) {
        for (u32 j = 0; j < v; ++j) std::cout << a[i * v + j] << ' ';
        std::cout << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
