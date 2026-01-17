#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/basic_link_cut_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::basic_link_cut_tree lct(n);
    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;
        lct.link(a, b);
    }

    while (q--) {
        u32 u, v, k;
        std::cin >> u >> v >> k;

        if (auto x = lct.jump(u, v, k))
            std::cout << *x << '\n';
        else
            std::cout << -1 << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
