#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binomial.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint1000000007;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::binomial<Z> z(2 * n);

    std::vector<std::pair<u32, u32>> p;
    p.reserve(m + 1);

    for (u32 i = 0; i < m; ++i) {
        u32 x, y;
        std::cin >> x >> y;

        p.emplace_back(--x, --y);
    }

    std::ranges::sort(p);
    p.emplace_back(n - 1, n - 1);

    std::vector<Z> dp(m + 1);
    for (u32 i = 0; i <= m; ++i) {
        auto &[x, y] = p[i];
        dp[i] = z.comb(x + y, x);

        for (u32 j = 0; j < i; ++j) {
            auto &[a, b] = p[j];
            if (b <= y) dp[i] -= dp[j] * z.comb(x + y - a - b, x - a);
        }
    }

    std::cout << dp[m].val();
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
