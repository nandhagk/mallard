#include <bits/stdc++.h>

#include "lib/compress.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/wavelet_matrix.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(v.size(), UNSET), nxt(n, n);
    for (u32 i = 0; i < n; ++i) {
        if (prv[na[i]] != UNSET) nxt[prv[na[i]]] = i;
        prv[na[i]] = i;
    }

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 0; u < n; ++u) e.emplace_back(nxt[u], u);
    mld::static_csr g(n + 1, e);
    mld::hld h(g, n);

    mld::wavelet_matrix wm(
        std::views::iota(u32{0}, n) |
            std::views::transform([&](u32 u) { return h.jump(u, k).value_or(n); }),
        n + 1);

    u32 q;
    std::cin >> q;

    for (u32 lst = 0; q--;) {
        u32 x, y;
        std::cin >> x >> y;

        u32 l = (x + lst) % n + 1;
        u32 r = (y + lst) % n + 1;
        if (l > r) std::swap(l, r);

        lst = r - l + 1 - wm.rank(l - 1, r, r);
        std::cout << lst << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
