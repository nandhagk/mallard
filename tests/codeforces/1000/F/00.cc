#include <bits/stdc++.h>
#include <limits>

#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/wavelet_matrix_aggregate.h"

static constexpr u32 N = 500'000;
static constexpr std::pair<i32, u32> INF{std::numeric_limits<i32>::max(), 0};

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, --x;

    constexpr u32 UNSET = -1;
    std::vector<u32> lst(N, UNSET), nxt(n, n);
    std::vector<i32> prv(n, -1);
    for (u32 i = 0; i < n; ++i) {
        if (lst[a[i]] != UNSET) nxt[lst[a[i]]] = i;
        prv[i] = lst[a[i]];
        lst[a[i]] = i;
    }

    mld::wavelet_matrix_aggregate<mld::four_russian<
        mld::sparse_table<mld::algebra::min<std::pair<i32, u32>, INF>>>>
        wma(std::views::iota(u32{0}, n) | std::views::transform([&](u32 k) {
                return std::make_pair(n - nxt[k], std::make_pair(prv[k], a[k]));
            }),
            n + 1);

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        auto [k, x] = wma.rank(--l, r, n - r + 1);
        auto [u, v] = x.val();
        if (k == 0 || u >= i32(l))
            std::cout << 0 << '\n';
        else
            std::cout << v + 1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
