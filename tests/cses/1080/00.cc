#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binomial.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());
    mld::binomial<Z> b(n + 1);

    mld::static_csr<Z> dp(std::views::iota(u32{0}, n + 1) |
                          std::views::transform([n](auto) { return n + 1; }));

    for (u32 i = 1; i <= n; ++i) dp[i][i - 1] = 1;
    for (u32 i = n; i--;) {
        for (u32 j = i + 1; j < n; j += 2) {
            for (u32 k = i + 1; k <= j; k += 2) {
                if (s[i] == s[k]) {
                    dp[i][j] += dp[i + 1][k - 1] * dp[k + 1][j] *
                                b.comb((j - i + 1) / 2, (k - i + 1) / 2);
                }
            }
        }
    }

    std::cout << dp[0][n - 1].val() << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
