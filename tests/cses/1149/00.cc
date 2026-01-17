#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    std::string s;
    std::cin >> s;

    std::array<Z, 26> dp{};
    Z ans = 1;
    for (auto &&c : s | std::views::transform([](char c) { return c - 'a'; })) {
        Z d = dp[c];
        dp[c] = ans;
        ans += ans - d;
    }

    std::cout << (ans - 1).val() << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
