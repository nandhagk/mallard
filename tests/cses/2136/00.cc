#include <bits/stdc++.h>

#pragma GCC target("popcnt")

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> v(n);
    for (u32 i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;

        for (u32 j = 0; j < k; ++j) v[i] |= (s[j] - '0') << j;
    }

    u32 ans = k;
    for (u32 i = 0; i < n; ++i)
        for (u32 j = i + 1; j < n; ++j)
            ans = std::min<u32>(ans, std::popcount(v[i] ^ v[j]));

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
