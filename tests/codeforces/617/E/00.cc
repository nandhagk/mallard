#include <bits/stdc++.h>

#include "lib/algebra/lxor.h"
#include "lib/offline/mo.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"

static constexpr u32 K = 1'000'000;

void solve() {
    u32 n, q, k;
    std::cin >> n >> q >> k;

    mld::scan_table<mld::algebra::lxor<u32>> a(std::views::iota(u32{0}, n) |
                                               std::views::transform([](auto) {
                                                   u32 x;
                                                   std::cin >> x;
                                                   return x;
                                               }));

    std::vector<u64> ans(q);

    mld::offline::mo m(n);
    m.reserve(q);

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        m.query(--l, r);
    }

    std::vector<u32> cnt(std::bit_ceil(K));
    ++cnt[0];

    u64 cur = 0;
    m.solve(
        [&](u32 j) {
            u32 z = j == 0 ? 0 : a.fold(j).val();
            cur += cnt[z ^ k], ++cnt[z];
        },
        [&](u32 j) {
            u32 z = a.fold(j + 1).val();
            cur += cnt[z ^ k], ++cnt[z];
        },
        [&](u32 j) {
            u32 z = j == 0 ? 0 : a.fold(j).val();
            --cnt[z], cur -= cnt[z ^ k];
        },
        [&](u32 j) {
            u32 z = a.fold(j + 1).val();
            --cnt[z], cur -= cnt[z ^ k];
        },
        [&](u32 j) { ans[j] = cur; });

    for (auto &&x : ans) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
