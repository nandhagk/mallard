#include <bits/stdc++.h>

#pragma GCC target("popcnt")

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::bitset<3072>> a;
    a.reserve(n);

    std::string s;
    s.reserve(n + 1);

    for (u32 i = 0; i < n; ++i) std::cin >> s, a.emplace_back(s);

    u64 ans = 0;
    for (u32 i = 0; i < n; ++i) {
        for (u32 j = i + 1; j < n; ++j) {
            u64 z = (a[i] & a[j]).count();
            ans += z * (z - 1);
        }
    }

    std::cout << (ans >> 1) << '\n';
}

i32 main() {
    solve();
}
