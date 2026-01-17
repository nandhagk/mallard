#include <bits/stdc++.h>

#include "lib/algebra/max.h"
#include "lib/compress.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/sparse_table.h"
#include "lib/wavelet_matrix_aggregate.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    auto [z, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(z.size(), UNSET), nxt(2 * n, 2 * n);
    for (u32 i = 0; i < 2 * n; ++i) {
        u32 u = h.tor[i >= n ? i - n : i];
        if (prv[na[u]] != UNSET) nxt[prv[na[u]]] = i;
        prv[na[u]] = i;
    }

    mld::wavelet_matrix_aggregate<
        mld::four_russian<mld::sparse_table<mld::algebra::max<u32>>>>
        wma(std::views::iota(u32{0}, 2 * n) | std::views::transform([&](u32 k) {
                return std::make_pair(nxt[k], na[h.tor[k >= n ? k - n : k]]);
            }),
            2 * n + 1);

    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = e[i << 1];
        if (h.par[u] == v) std::swap(u, v);

        u32 l = h.tin[v], r = h.tin[v] + h.siz[v];
        auto [x, y] = wma.rank(l, r, r);
        auto [a, b] = wma.rank(r, l + n, l + n);

        u32 ans = 0;
        if (x) ans = std::max(ans, z[y.val()]);
        if (a) ans = std::max(ans, z[b.val()]);
        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
