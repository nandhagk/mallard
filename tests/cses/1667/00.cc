#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);

    constexpr u32 UNSET = -1;
    std::vector<u32> d(n, UNSET), p(n, UNSET);
    std::queue<u32> q;

    q.push(n - 1);
    d[n - 1] = 0;

    while (!q.empty()) {
        u32 u = q.front();
        q.pop();

        for (u32 v : g[u])
            if (d[v] == UNSET) d[v] = d[u] + 1, p[v] = u, q.push(v);
    }

    if (d[0] == UNSET) {
        std::cout << "IMPOSSIBLE\n";
        return;
    }

    std::cout << d[0] + 1 << '\n';
    for (u32 cur = 0; cur != UNSET; cur = p[cur]) std::cout << cur + 1 << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
