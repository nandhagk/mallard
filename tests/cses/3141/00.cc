#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n;
    std::cin >> n;

    u32 k = std::bit_width(n);
    std::vector<u32> dp(1 << k);
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        ++dp[a];
    }

    std::vector<Z> pw(n + 1);
    pw[0] = 1;
    for (u32 i = 1; i <= n; ++i) pw[i] = pw[i - 1] * 2;

    for (u32 i = 0; i < k; ++i) {
        for (u32 j = 0; j < (1 << k); ++j)
            if (!((j >> i) & 1)) dp[j] += dp[j ^ (1 << i)];
    }

    std::vector<Z> kp(1 << k);
    for (u32 i = 0; i < (1 << k); ++i) kp[i] = pw[dp[i]] - 1;

    for (u32 i = 0; i < k; ++i) {
        for (u32 j = 0; j < (1 << k); ++j)
            if (!((j >> i) & 1)) kp[j] -= kp[j ^ (1 << i)];
    }

    for (u32 i = 0; i <= n; ++i) std::cout << kp[i].val() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
