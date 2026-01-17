#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    std::vector<u32> a(n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--v, --u);
        ++a[u];
    }

    mld::static_csr h(n, e);

    std::priority_queue<u32> q;
    for (u32 u = 0; u < n; ++u)
        if (a[u] == 0) q.push(u);

    std::vector<u32> ans(n);
    for (u32 u = n; u--;) {
        u32 j = q.top();
        q.pop();

        for (u32 v : h[j])
            if (--a[v] == 0) q.push(v);

        ans[u] = j;
    }

    for (auto &&x : ans) std::cout << x + 1 << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
