#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/radix_heap.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u64>>> e;
    e.reserve(m);

    while (m--) {
        u32 a, b, c;
        std::cin >> a >> b >> c;

        e.emplace_back(--a, std::pair<u32, u64>{--b, c});
    }

    mld::static_csr g(n, e);

    std::vector<bool> vis(n);
    std::vector<std::tuple<u64, Z, u32, u32>> dst(n, {std::numeric_limits<u64>::max(), 0, 0, 0});
    mld::radix_heap<u64, u32> que;

    dst[0] = {0, 1, 0, 0};
    que.emplace(0, 0);

    while (!que.empty()) {
        auto [_, u] = que.top();
        que.pop();

        if (vis[u]) continue;
        vis[u] = true;

        for (auto &&[v, w] : g[u]) {
            auto &&[a, b, c, d] = dst[u];
            auto &&[p, r, s, t] = dst[v];

            if (a + w < p) {
                p = a + w;
                r = b;
                s = c + 1;
                t = d + 1;
                que.emplace(p, v);
            } else if (a + w == p) {
                r += b;
                s = std::min(s, c + 1);
                t = std::max(t, d + 1);
            }
        }
    }

    auto &&[a, b, c, d] = dst[n - 1];
    std::cout << a << ' ' << b.val() << ' ' << c << ' ' << d << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
