#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/am_tree.h"
#include "lib/erasable_heap.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::am_tree<std::pair<u32, u32>> amt(n);
    mld::erasable_heap<std::pair<u32, u32>> hep;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    u32 cnt = n;
    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, --v);

        u32 a = amt.size(u), b = amt.size(v);
        if (auto res = amt.insert(u, v, {w, i})) {
            if (auto r = *res)
                hep.erase(*r);
            else if ((a % 2) && (b % 2))
                cnt -= 2;

            hep.insert(w, i);
        }

        if (cnt == 0) {
            for (;;) {
                auto [x, j] = hep.top();
                auto &&[s, t] = e[j];
                if (!amt.erase(s, t, {x - 1, j})) break;

                if (amt.size(s) % 2 || amt.size(t) % 2) {
                    amt.insert(s, t, {x, j});
                    break;
                } else {
                    hep.pop();
                }
            }

            std::cout << hep.top().first << '\n';
        } else {
            std::cout << -1 << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
