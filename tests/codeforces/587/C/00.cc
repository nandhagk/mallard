#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"
#include "lib/wavelet_matrix.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

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

    std::vector<std::pair<u32, u32>> f;
    f.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 c;
        std::cin >> c;

        f.emplace_back(--c, i);
    }

    mld::static_csr h(n, f);

    mld::hld_aggregate<mld::scan_table<mld::algebra::sum<u32>>> hag(
        g, std::views::iota(u32{0}, n) | std::views::transform([&](u32 u) {
               return static_cast<u32>(h[u].size());
           }));

    std::vector<u32> tin(n);

    std::vector<u32> a;
    a.reserve(m + n);

    for (u32 u : hag.tor) {
        tin[u] = static_cast<u32>(a.size());

        a.push_back(m);
        for (u32 x : h[u]) a.push_back(x);
    }

    mld::wavelet_matrix wm(a, m + 1);

    while (q--) {
        u32 u, v, x;
        std::cin >> u >> v >> x;

        x = std::min(x, hag.fold_path(--u, --v).val());
        if (x == 0) {
            std::cout << 0 << '\n';
            continue;
        }

        auto segments = hag.decompose(u, v);
        for (auto &&[s, t] : segments) {
            if (tin[s] > tin[t]) std::swap(s, t);
            s = tin[s], t = tin[t] + static_cast<u32>(h[t].size()) + 1;
        }

        std::cout << x;
        for (u32 i = 0; i < x; ++i) std::cout << ' ' << wm.kth(segments, i) + 1;

        std::cout << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
