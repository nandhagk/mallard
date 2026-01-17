#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/mo.h"
#include "lib/prelude.h"

static constexpr u32 N = 1'000'000;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<u64> ans(q);

    mld::offline::mo m(n);
    m.reserve(q);

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        m.query(--l, r);
    }

    std::vector<u32> cnt(N + 1);

    u64 cur = 0;
    m.solve(
        [&](u32 k) {
            u32 s = a[k];
            cur -= u64(s) * cnt[s] * cnt[s];
            ++cnt[s];
            cur += u64(s) * cnt[s] * cnt[s];
        },
        [&](u32 k) {
            u32 s = a[k];
            cur -= u64(s) * cnt[s] * cnt[s];
            --cnt[s];
            cur += u64(s) * cnt[s] * cnt[s];
        },
        [&](u32 k) { ans[k] = cur; });

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
