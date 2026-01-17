#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/manhattan_mst.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<i32, i32>> p(n);
    for (auto &&[x, y] : p) {
        u32 a, b;
        std::cin >> a >> b;
        x = a, y = b;
    }

    mld::am_tree<std::pair<u32, u32>> amt(n);

    std::vector<std::pair<u32, u32>> e;
    std::vector<bool> edg;

    e.reserve(4 * n), edg.reserve(4 * n);

    u64 ans = 0;
    mld::manhattan_mst(p, [&](u32 i, u32 j) {
        auto &&[a, b] = p[i];
        auto &&[u, v] = p[j];

        u32 k = e.size();
        e.emplace_back(i, j);

        u32 w = u32(std::abs(a - u)) + u32(std::abs(b - v));
        if (auto res = amt.insert(i, j, {w, k})) {
            if (auto r = *res) edg[r->second] = false, ans -= r->first;
            edg.push_back(true), ans += w;
        } else {
            edg.push_back(false);
        }
    });

    std::cout << ans << '\n';
    for (u32 i = 0; i < e.size(); ++i) {
        if (!edg[i]) continue;

        auto &&[u, v] = e[i];
        std::cout << u << ' ' << v << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
