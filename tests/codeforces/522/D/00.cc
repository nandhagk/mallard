#include <bits/stdc++.h>

#include "lib/algebra/min.h"
#include "lib/compress.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/wavelet_matrix_aggregate.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          i32 a;
                          std::cin >> a;
                          return a;
                      }));

    u32 m = static_cast<u32>(v.size());

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(m, UNSET), nxt(n, n);
    for (u32 i = 0; i < n; ++i) {
        if (prv[na[i]] != UNSET) nxt[prv[na[i]]] = i;
        prv[na[i]] = i;
    }

    mld::wavelet_matrix_aggregate<
        mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>>>
        wma(std::views::iota(u32{0}, n) | std::views::transform([&](u32 k) {
                u32 z = nxt[k] == n ? n : nxt[k] - k;
                return std::make_pair(nxt[k], z);
            }),
            n + 1);

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        auto [c, x] = wma.rank(--l, r, r);
        std::cout << i32(x == n ? -1 : x.val()) << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
