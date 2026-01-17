#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> w(m);
    for (auto &&x : w) std::cin >> x;

    std::vector<u32> c(m);
    for (auto &&x : c) std::cin >> x;

    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;

    mld::am_tree<std::pair<u32, u32>> amt(n);

    u64 ans = 0;
    std::vector<bool> edg(m);
    for (u32 i = 0; i < m; ++i) {
        auto &&[u, v] = e[i];
        if (auto res = amt.insert(u, v, {w[i], i})) {
            if (auto r = *res) ans -= r->first, edg[r->second] = 0;
            ans += w[i], edg[i] = 1;
        }
    }

    u32 s;
    std::cin >> s;

    std::vector<i64> ord(m);
    for (u32 i = 0; i < m; ++i) {
        auto &&[u, v] = e[i];
        i64 z = i64(w[i]) - s / c[i];
        ord[i] = z - i64(amt.max_path(u, v)->first);
    }

    u32 k = static_cast<u32>(std::ranges::min_element(ord) - ord.begin());
    {
        auto &&[u, v] = e[k];
        auto [y, j] = *amt.max_path(u, v);
        ans -= y, edg[j] = 0;

        i64 z = i64(w[k]) - s / c[k];
        ans += z, edg[k] = 1;
    }

    std::cout << i64(ans) << '\n';
    for (u32 i = 0; i < m; ++i) {
        if (!edg[i]) continue;
        if (i == k)
            std::cout << i + 1 << ' ' << i64(w[k]) - s / c[k] << '\n';
        else
            std::cout << i + 1 << ' ' << w[i] << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
