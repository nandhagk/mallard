#include <bits/stdc++.h>

#include "lib/manacher.h"
#include "lib/prelude.h"
#include "lib/z_algorithm.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto z = mld::z_algorithm(s);
    auto [o, e] = mld::manacher(s);

    u32 n = static_cast<u32>(s.size());
    std::vector<u32> dp(n);

    dp[0] = 1;
    for (u32 i = 1; i < n; ++i) {
        if (i % 2) {
            if (e[i / 2 + 1] != i / 2 + 1) continue;
            dp[i] = 1 + dp[i / 2] * (z[i / 2 + 1] >= i / 2);
        } else {
            if (o[i / 2] != i / 2 + 1) continue;
            dp[i] = 1 + dp[i / 2 - 1] * (z[i / 2 + 1] >= i / 2);
        }
    }

    std::cout << std::reduce(dp.begin(), dp.end(), u64{0}) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
