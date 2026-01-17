#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/manhattan_mst.h"
#include "lib/prelude.h"
#include "lib/union_find.h"
#include "ska_sort.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<i32, i32>> p(n);
    for (auto &&[x, y] : p) {
        u32 a, b;
        std::cin >> a >> b;
        x = a, y = b;
    }

    std::vector<std::pair<u32, u32>> y, z;
    y.reserve(4 * n), z.reserve(4 * n);

    mld::manhattan_mst(p, [&](u32 i, u32 j) {
        auto &&[a, b] = p[i];
        auto &&[u, v] = p[j];
        z.emplace_back(u32(std::abs(a - u)) + u32(std::abs(b - v)), y.size());
        y.emplace_back(i, j);
    });

    ska::ska_sort(z.begin(), z.end());

    std::vector<bool> edg(z.size());
    mld::union_find uf(n);

    u64 ans = 0;
    for (auto &&[w, i] : z) {
        auto &&[u, v] = y[i];
        if (uf.merge(u, v)) ans += w, edg[i] = true;
    }

    std::cout << ans << '\n';
    for (u32 i = 0; i < edg.size(); ++i) {
        if (!edg[i]) continue;

        auto &&[u, v] = y[i];
        std::cout << u << ' ' << v << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
