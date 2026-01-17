#include <bits/stdc++.h>
#include <limits>

#include "blazingio.min.h"
#include "lib/static_csr.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::vector<bool> xs(n);
    for (u32 i = 0; i < k; ++i) {
        u32 u;
        std::cin >> u;

        xs[--u] = true;
    }

    std::vector<std::pair<u32, u32>> h;
    h.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        h.emplace_back(u, v);
        h.emplace_back(v, u);
    }

    mld::static_csr g(n, h);
    std::vector<u32> d(n, std::numeric_limits<u32>::max() / 2), e(n, n), p(n, n);

    std::queue<u32> q;
    std::vector<bool> s(n);

    for (u32 u = 0; u < n; ++u) {
        if (!xs[u]) continue;

        d[u] = 0;
        p[u] = u;
        s[u] = true;
        q.push(u);
    }

    while (!q.empty()) {
        u32 u = q.front();
        q.pop();

        for (u32 v : g[u]) {
            if (s[v]) {
                if (p[v] != p[u]) {
                    e[p[u]] = std::min(e[p[u]], d[u] + 1 + d[v]);
                    e[p[v]] = std::min(e[p[v]], d[u] + 1 + d[v]);
                }
                continue;
            }

            d[v] = d[u] + 1;
            e[v] = d[v];
            s[v] = true;
            p[v] = p[u];
            q.push(v);
        }
    }

    for (u32 u = 0; u < n; ++u) std::cout << i32(e[u] == n ? -1 : e[u]) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
