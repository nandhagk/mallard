#include <bits/stdc++.h>

#include "lib/offline/mo.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<u32> ans(q);

    mld::offline::mo m(n);
    m.reserve(q);

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        m.query(--l, r);
    }

    std::vector<u32> cnt(n + 1);

    u32 cur = 0;
    m.solve(
        [&](u32 i) {
            if (a[i] > n) return;
            if (u32 z = ++cnt[a[i]]; z == a[i])
                ++cur;
            else if (z == a[i] + 1)
                --cur;
        },
        [&](u32 i) {
            if (a[i] > n) return;
            if (u32 z = cnt[a[i]]--; z == a[i] + 1)
                ++cur;
            else if (z == a[i])
                --cur;
        },
        [&](u32 i) { ans[i] = cur; });

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
