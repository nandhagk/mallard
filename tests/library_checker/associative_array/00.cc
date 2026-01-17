#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/pbds.h"

void solve() {
    u32 q;
    std::cin >> q;

    mld::pbds::hash_map<u64, u64> hm;
    hm.resize(q);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u64 k, v;
            std::cin >> k >> v;

            hm[k] = v;
        } else {
            u64 k;
            std::cin >> k;

            std::cout << hm[k] << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
