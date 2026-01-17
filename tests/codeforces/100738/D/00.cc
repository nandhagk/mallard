#include <bits/stdc++.h>

#include "lib/prelude.h"

// https://web.iitd.ac.in/~atripath/publications/graphic_trees.pdf
void solve() {
    u32 n;
    std::cin >> n;

    u64 tot = 0;

    std::set<std::pair<u32, u32>> deg;
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        tot += a;
        deg.emplace(a, i);
    }

    if (tot != 2 * (n - 1)) {
        std::cout << -1 << '\n';
        return;
    }

    if (n == 1) return;

    while (!deg.empty()) {
        auto it = deg.begin();
        auto ti = deg.rbegin();

        assert(it->first == 1);
        std::cout << it->second + 1 << ' ' << ti->second + 1 << '\n';

        if (ti->first > 1) deg.emplace(ti->first - 1, ti->second);
        deg.erase(*ti);
        deg.erase(it);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
