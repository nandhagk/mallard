#include <bits/stdc++.h>
#include <functional>

#include "lib/parallel_union_find.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"
#include "lib/union_find.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::string s;
    std::cin >> s;

    mld::union_find uf(n);
    mld::parallel_union_find puf(2 * n);

    auto f = [&](u32 i, u32 j) {
        if (i >= n) i = 2 * n - 1 - i;
        if (j >= n) j = 2 * n - 1 - j;

        uf.merge(i, j);
    };

    while (m--) {
        u32 l, r;
        std::cin >> l >> r;

        --l, --r;
        puf.merge(l, 2 * n - r - 1, r - l + 1, f);
    }

    constexpr u32 UNSET = -1;
    std::vector<u32> pos(n, UNSET);
    for (u32 i = 0; i < n; ++i) {
        if (s[i] == '?') continue;

        u32 j = uf.find(i);
        if (pos[j] == UNSET) pos[j] = s[i] - '0';
        if (pos[j] != u32(s[i] - '0')) {
            std::cout << 0 << '\n';
            return;
        }
    }

    std::vector<Z> dp(n, 1);
    for (u32 i = 0; i < n; ++i) dp[uf.find(i)] = 2;

    for (u32 i = 0; i < n; ++i)
        if (s[i] != '?') dp[uf.find(i)] = 1;

    std::cout << std::reduce(dp.begin(), dp.end(), Z(1), std::multiplies<>{}).val()
              << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
