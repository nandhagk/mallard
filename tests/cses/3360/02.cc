#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::string z;
    z.reserve(n + 1);

    mld::static_csr<std::bitset<3072>> s(
        std::views::iota(u32{0}, k) | std::views::transform([n](auto) { return n; }));

    std::vector<bool> ans(k);
    for (u32 i = 0; i < n; ++i) {
        std::cin >> z;
        std::vector<std::bitset<3072>> t(k);
        for (u32 j = 0; j < n; ++j) {
            u32 y = z[j] - 'A';
            if (ans[y]) continue;

            if ((s[y][j] & t[y]).any()) ans[y] = true;

            s[y][j] |= t[y];
            t[y].set(j);
        }
    }

    for (auto &&x : ans) std::cout << (x ? "YES\n" : "NO\n");
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
