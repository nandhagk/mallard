#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n), b(n);
    for (auto &&x : a) std::cin >> x, --x;

    std::vector<u32> c(n);
    for (u32 i = 0; i < n; ++i) {
        std::cin >> b[i];
        c[--b[i]] = i;
    }

    u32 j = 0;
    auto dfs = [&](auto &&self, u32 l, u32 r) -> void {
        if (l >= r) {
            --j;
            return;
        }

        u32 k = c[a[j]];
        ++j, self(self, l, k);
        ++j, self(self, k + 1, r);
        std::cout << b[k] + 1 << ' ';
    };

    dfs(dfs, 0, n);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
