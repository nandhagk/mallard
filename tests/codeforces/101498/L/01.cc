#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/spfa.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, i64>>> e;
    e.reserve(m + n);

    std::vector<std::pair<u32, std::pair<u32, u64>>> f;
    f.reserve(m);

    i64 ans = std::numeric_limits<i64>::max();
    while (m--) {
        u32 u, v;
        i32 w;
        std::cin >> u >> v >> w;

        ans = std::min<i64>(ans, w);
        e.emplace_back(--u, std::pair<u32, i64>{--v, w});
    }

    for (u32 u = 0; u < n; ++u) e.emplace_back(n, std::pair<u32, i64>{u, 0});
    mld::static_csr g(n + 1, e);

    auto [dst, _] = mld::spfa(g, n);

    i64 z = *std::ranges::min_element(dst);
    if (z < 0) ans = std::min(ans, z);

    if (ans == std::numeric_limits<i64>::min())
        std::cout << "-inf\n";
    else
        std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
