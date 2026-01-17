#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/z_algorithm.h"

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());

    auto z = mld::z_algorithm(s);
    std::vector<u32> cnt(n + 1);
    for (auto &&x : z) ++cnt[x];

    std::vector<std::pair<u32, u32>> ans;
    ans.reserve(n);

    for (u32 x = n, p = 0; x > 0; --x) {
        p += cnt[x];
        if (z[n - x] == x) ans.emplace_back(x, p);
    }

    std::cout << ans.size() << '\n';
    for (auto &&[l, c] : ans | std::views::reverse) std::cout << l << ' ' << c << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
