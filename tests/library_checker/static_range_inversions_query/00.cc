#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/fenwick_set.h"
#include "lib/offline/mo.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> a;
    a.reserve(n);

    for (u32 i = 0; i < n; ++i) {
        u32 x;
        std::cin >> x;
        a.emplace_back(x, i);
    }

    std::ranges::sort(a);

    std::vector<u32> na(n);
    for (u32 i = 0; i < n; ++i) na[a[i].second] = i;

    mld::offline::mo mo(n);
    mo.reserve(q);

    std::vector<u64> ans(q);
    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        mo.query(l, r);
    }

    mld::fenwick_set<> fws(n);

    u64 cur = 0, siz = 0;
    mo.solve([&](u32 i) { cur += fws.rank(na[i]), fws.insert(na[i]), ++siz; },
             [&](u32 i) { cur += siz - fws.rank(na[i] + 1), fws.insert(na[i]), ++siz; },
             [&](u32 i) { cur -= fws.rank(na[i]), fws.erase(na[i]), --siz; },
             [&](u32 i) { cur -= siz - fws.rank(na[i] + 1), fws.erase(na[i]), --siz; },
             [&](u32 i) { ans[i] = cur; });

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
