#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/rooted_tree_lca.h"
#include "lib/static_csr.h"
#include "lib/wavelet_matrix.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;

        e.emplace_back(--p, u);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree_lca h(g);

    mld::wavelet_matrix wm(h.tin, n);
    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        --l;
        u32 u = h.tor[wm.kth(l, r, 0)];
        u32 w = h.tor[wm.kth(l, r, 1)];
        u32 v = h.tor[wm.kth(l, r, r - l - 1)];
        u32 x = h.tor[wm.kth(l, r, r - l - 2)];

        u32 a = h.lca(w, v);
        u32 b = h.lca(u, x);
        if (h.dep[a] >= h.dep[b]) {
            std::cout << u + 1 << ' ' << h.dep[a] << '\n';
        } else {
            std::cout << v + 1 << ' ' << h.dep[b] << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
