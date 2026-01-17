#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/max.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/wavelet_matrix_aggregate.h"

static constexpr u32 N = 100'000;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, --x;

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(n), nxt(N, UNSET);
    for (u32 i = n; i--;) {
        if (nxt[a[i]] != UNSET) prv[nxt[a[i]]] = i + 1;
        nxt[a[i]] = i;
    }

    std::vector<u32> lst(N, UNSET);
    nxt.assign(n, n);
    for (u32 i = 0; i < n; ++i) {
        if (lst[a[i]] != UNSET) nxt[lst[a[i]]] = i;
        lst[a[i]] = i;
    }

    std::vector<u32> arp(n, n);
    for (u32 i = n; i--;) {
        if (nxt[i] == n || nxt[nxt[i]] == n) continue;

        arp[i] = nxt[nxt[i]] - 1;
        if (nxt[nxt[i]] - nxt[i] == nxt[i] - i) arp[i] = arp[nxt[i]];
    }

    mld::wavelet_matrix_aggregate<
        mld::four_russian<mld::sparse_table<mld::algebra::max<u32>>>>
        wma(std::views::iota(u32{0}, n) | std::views::transform([&](u32 k) {
                return std::make_pair(prv[k], arp[k]);
            }),
            n + 1);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        --l;
        auto [x, y] = wma.rank(l, r, l + 1);
        std::cout << x + (y < r - 1) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
