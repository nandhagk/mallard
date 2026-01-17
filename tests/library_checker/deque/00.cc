#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 q;
    std::cin >> q;

    std::deque<u32> d;
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            u32 x;
            std::cin >> x;

            d.push_front(x);
        } else if (t == 1) {
            u32 x;
            std::cin >> x;

            d.push_back(x);
        } else if (t == 2) {
            d.pop_front();
        } else if (t == 3) {
            d.pop_back();
        } else {
            u32 i;
            std::cin >> i;

            std::cout << d[i] << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
