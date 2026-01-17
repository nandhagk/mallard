#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/manhattan_mst.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<i32, i32>> p(n);
    for (auto &&[x, y] : p) {
        u32 a, b;
        std::cin >> a >> b;
        x = a, y = b;
    }

    std::vector<std::tuple<u32, u32, u32>> e;

    mld::manhattan_mst(p, [&](u32 i, u32 j) {
        auto &&[a, b] = p[i];
        auto &&[u, v] = p[j];
        e.emplace_back(u32(std::abs(a - u)) + u32(std::abs(b - v)), i, j);
    });

    std::ranges::sort(e);

    std::vector<bool> edg(e.size());
    mld::union_find uf(n);

    u64 ans = 0;

    u32 k = 0;
    for (auto &&[w, u, v] : e) {
        if (uf.merge(u, v)) ans += w, edg[k] = true;
        ++k;
    }

    std::cout << ans << '\n';
    for (u32 i = 0; i < edg.size(); ++i) {
        if (!edg[i]) continue;

        auto &&[_, u, v] = e[i];
        std::cout << u << ' ' << v << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
