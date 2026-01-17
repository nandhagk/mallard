#include <bits/stdc++.h>

#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    for (u32 i = 0; i < m; ++i) e.push_back(e[i]);

    std::vector<u32> ans(m);

    mld::am_tree<i32> amt(2 * n);
    for (u32 i = 0, j = 0; i < m; ++i) {
        for (; j < i + m; ++j) {
            auto &&[a, b] = e[j];
            amt.insert(a, b + n, -i32(j * 2 + 0));
            amt.insert(b, a + n, -i32(j * 2 + 1));
            if (amt.is_connected(a, a + n)) break;
        }

        ans[i] = j;
        auto &&[u, v] = e[i];
        amt.erase(u, v + n, -i32(i * 2 + 1));
        amt.erase(v, u + n, -i32(i * 2 + 2));
    }

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        --l;
        if (r == m)
            std::cout << (l > ans[0] ? "YES\n" : "NO\n");
        else
            std::cout << (l + m > ans[r] ? "YES\n" : "NO\n");
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
