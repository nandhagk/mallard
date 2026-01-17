#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/z_algorithm.h"

void solve() {
    std::string s, t;
    std::cin >> s >> t;

    std::string u = t + '#' + s;
    auto z = mld::z_algorithm(u);

    debug(u, z);
    u32 n = static_cast<u32>(s.size()), m = static_cast<u32>(t.size());

    u32 ans = 0;
    for (u32 i = 0; i < n; ++i)
        if (z[i + m + 1] == m) i += m - 1, ++ans;

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
