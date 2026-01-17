#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

static const constexpr i32 N = 20;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<u32> dp(1 << N), kp(1 << N);
    for (auto &&x : a) ++dp[x], ++kp[x];

    for (u32 i = 0; i < N; ++i) {
        for (u32 j = 0; j < (1 << N); ++j)
            if ((j >> i) & 1)
                dp[j] += dp[j ^ (1 << i)];
            else
                kp[j] += kp[j ^ (1 << i)];
    }

    for (auto &&x : a)
        std::cout << dp[x] << ' ' << kp[x] << ' ' << n - dp[((1 << N) - 1) ^ x] << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
