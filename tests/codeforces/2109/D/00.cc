#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, l;
    std::cin >> n >> m >> l;

    std::vector<u32> a(l), b, c;
    b.reserve(l), c.reserve(l);
    for (auto &&x : a) std::cin >> x;
    for (auto &&x : a)
        if (x % 2)
            b.push_back(x);
        else
            c.push_back(x);

    std::ranges::sort(b);
    std::ranges::sort(c);

    std::vector<std::pair<u32, u32>> e;
    e.reserve(4 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u * 2 + 0, v * 2 + 1);
        e.emplace_back(v * 2 + 0, u * 2 + 1);

        e.emplace_back(u * 2 + 1, v * 2 + 0);
        e.emplace_back(v * 2 + 1, u * 2 + 0);
    }

    mld::static_csr g(2 * n, e);

    std::queue<u32> q;

    constexpr u32 UNSET = -1;
    std::vector<u32> d(2 * n, UNSET);

    q.push(0), d[0] = 0;
    while (!q.empty()) {
        u32 u = q.front();
        q.pop();

        for (u32 v : g[u])
            if (d[v] == UNSET) d[v] = d[u] + 1, q.push(v);
    }

    u32 x = std::reduce(a.begin(), a.end());
    u32 s = std::reduce(b.begin(), b.end());
    u32 t = x - s;

    for (u32 u = 0; u < n; ++u) {
        if (u32 z = d[u * 2 + 0];
            z != UNSET && z <= t + (b.size() % 2 ? s - b[0] : s)) {
            std::cout << '1';
        } else if (z = d[u * 2 + 1];
                   z != UNSET && !b.empty() && z <= t + (b.size() % 2 ? s : s - b[0])) {
            std::cout << '1';
        } else {
            std::cout << '0';
        }
    }

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
