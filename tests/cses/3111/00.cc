#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"
#include "ska_sort.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::tuple<i32, u32, u32>> e;
    e.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(-w, --u, --v);
    }

    ska::ska_sort(e.begin(), e.end());
    mld::union_find uf(n);

    u64 ans = 0;
    for (auto &&[w, u, v] : e) {
        ans += static_cast<u64>(-w) * uf.size(u) * uf.size(v);
        uf.merge(u, v);
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
