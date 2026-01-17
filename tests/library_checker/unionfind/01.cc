#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/am_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::am_tree<u32> amt(n);
    while (q--) {
        u32 t, u, v;
        std::cin >> t >> u >> v;

        if (t == 0)
            amt.insert(u, v, 0);
        else
            std::cout << amt.is_connected(u, v) << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
