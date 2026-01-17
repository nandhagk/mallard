#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/suffix_array_query.h"

void solve() {
    std::string s;
    std::cin >> s;

    mld::suffix_array_query saq(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);

    u32 n = static_cast<u32>(s.size());

    std::vector<u32> t(n + 1);
    ++t[1];
    if (n >= 10) ++t[10];
    if (n >= 100) ++t[100];
    if (n >= 1000) ++t[1000];
    std::inclusive_scan(t.begin(), t.end(), t.begin());

    std::vector<u32> dp(n + 1);
    for (u32 i = 0; i <= n; ++i) dp[i] = 2 * i;

    for (u32 i = 0; i < n; ++i) {
        for (u32 x = 1; x <= n; ++x) {
            for (u32 y = x, k = 1; y + i <= n; y += x, ++k) {
                if (!saq.equate_substr(i, i + y - x, x)) break;
                dp[i + y] = std::min(dp[i + y], dp[i] + t[k] + x);
            }
        }
    }

    std::cout << dp[n] << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
