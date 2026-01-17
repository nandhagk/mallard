#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binary_trie.h"
#include "lib/managers/reusing.h"
#include "lib/prelude.h"

void solve() {
    u32 q;
    std::cin >> q;

    mld::binary_trie<u32, mld::managers::reusing<>> bt;
    while (q--) {
        u32 t, x;
        std::cin >> t >> x;

        if (t == 0)
            bt.insert(x);
        else if (t == 1)
            bt.erase(x);
        else
            std::cout << (x ^ bt.xor_min(x)) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
