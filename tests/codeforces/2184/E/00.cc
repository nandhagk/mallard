#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 i = 1; i < n; ++i)
        e.emplace_back(a[i] > a[i - 1] ? a[i] - a[i - 1] : a[i - 1] - a[i], i);
    mld::static_csr g(n, e);

    mld::union_find uf(n);

    std::vector<u64> ans(n);

    u64 cur = 0;
    for (u32 k = n - 1; k > 0; --k) {
        for (u32 j : g[k]) {
            u32 x = uf.size(j), y = uf.size(j - 1);
            if (uf.merge(j, j - 1)) {
                cur -= u64(x) * (x - 1) / 2;
                cur -= u64(y) * (y - 1) / 2;
                u32 z = uf.size(j);
                cur += u64(z) * (z - 1) / 2;
            }
        }

        ans[k] = cur;
    }

    for (auto &&x : ans | std::views::drop(1)) std::cout << x << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
