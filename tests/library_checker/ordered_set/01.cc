#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binary_trie.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"


// TODO: This fails for some reason while 00 passes ?!

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::binary_trie<u32, mld::managers::reusing<>> bt;
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        bt.insert(a);
    }

    while (q--) {
        u32 t, x;
        std::cin >> t >> x;
        if (t == 0) {
            if (!bt.count(x)) bt.insert(x);
        } else if (t == 1) {
            bt.erase(x);
        } else if (t == 2) {
            if (--x; x < bt.size())
                std::cout << bt.kth(x) << '\n';
            else
                std::cout << -1 << '\n';
        } else if (t == 3) {
            std::cout << bt.rank(++x) << '\n';
        } else if (t == 4) {
            if (auto y = bt.less_bound(x))
                std::cout << *y << '\n';
            else
                std::cout << -1 << '\n';
        } else //
            if (auto y = bt.lower_bound(x)) {
                std::cout << *y << '\n';
            } else {
                std::cout << -1 << '\n';
            }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
