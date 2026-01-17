#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/wavelet_matrix.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    auto [z, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

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
    mld::hld h(g);

    mld::wavelet_matrix wm(h.tor | std::views::transform([&](u32 k) { return na[k]; }), static_cast<u32>(z.size()));

    while (q--) {
        u32 u, v, k;
        std::cin >> u >> v >> k;

        auto segments = h.decompose(--u, --v);
        for (auto &&[l, r] : segments) std::tie(l, r) = std::minmax(h.tin[l], h.tin[r]), ++r;

        std::cout << z[wm.kth(segments, --k)] << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
