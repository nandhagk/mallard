#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/basic_link_cut_tree.h"
#include "lib/prelude.h"

// Remove all constexpr from basic_link_cut_tree.h
// to make it compile on cses
void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::basic_link_cut_tree lct(n);
    for (u32 u = 1; u < n; ++u) {
        u32 p;
        std::cin >> p;
        lct.link(u, --p);
    }

    while (q--) {
        u32 u, k;
        std::cin >> u >> k;

        if (auto x = lct.jump(--u, 0, k)) {
            std::cout << *x + 1 << '\n';
        } else {
            std::cout << -1 << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
