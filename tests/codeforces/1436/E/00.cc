#include <bits/stdc++.h>

#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/static_mex.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, i32>> e;
    e.reserve(3 * n);

    for (u32 i = 0; i <= n; ++i) e.emplace_back(i, -1);

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, e.emplace_back(--x, e.size() - n - 1);

    for (u32 i = 0; i <= n; ++i) e.emplace_back(i, n);

    std::ranges::reverse(e);
    mld::static_csr g(n + 1, e);

    mld::static_mex<mld::managers::clonable<>> sm(a);
    for (u32 i = 0; i <= n; ++i) {
        u32 k = static_cast<u32>(g[i].size());

        for (u32 j = 1; j < k; ++j)
            if (g[i][j - 1] + 1 != g[i][j] && sm.mex(g[i][j - 1] + 1, g[i][j]) == i)
                goto found;

        std::cout << i + 1 << '\n';
        return;

    found:;
    }

    std::cout << n + 2 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
