#include <bits/stdc++.h>
#include <limits>

#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, s, t, x, y;
    std::cin >> n >> m >> s >> t >> x >> y;

    --s, --t;

    std::vector<std::string> g(n);
    for (auto &&r : g) r.reserve(m + 1), std::cin >> r;

    mld::static_csr<u32> dst(std::views::iota(u32{0}, n) |
                             std::views::transform([m](auto) { return m; }));
    for (u32 i = 0; i < n; ++i)
        std::ranges::fill(dst[i], std::numeric_limits<u32>::max());

    std::deque<std::pair<u32, u32>> que;

    dst[s][t] = 0, que.emplace_back(s, t);
    while (!que.empty()) {
        auto [i, j] = que.front();
        que.pop_front();

        if (u32 w = 0; i > 0 && g[i - 1][j] != '*' && dst[i][j] + w < dst[i - 1][j])
            dst[i - 1][j] = dst[i][j] + w, que.emplace_front(i - 1, j);

        if (u32 w = 0; i < n - 1 && g[i + 1][j] != '*' && dst[i][j] + w < dst[i + 1][j])
            dst[i + 1][j] = dst[i][j] + w, que.emplace_front(i + 1, j);

        if (u32 w = 1; j > 0 && g[i][j - 1] != '*' && dst[i][j] + w < dst[i][j - 1])
            dst[i][j - 1] = dst[i][j] + w, que.emplace_back(i, j - 1);

        if (u32 w = 0; j < m - 1 && g[i][j + 1] != '*' && dst[i][j] + w < dst[i][j + 1])
            dst[i][j + 1] = dst[i][j] + w, que.emplace_front(i, j + 1);
    }

    u32 ans = 0;
    for (u32 i = 0; i < n; ++i) {
        for (u32 j = 0; j < m; ++j)
            if (dst[i][j] <= x && j + dst[i][j] <= y + t) ++ans;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
