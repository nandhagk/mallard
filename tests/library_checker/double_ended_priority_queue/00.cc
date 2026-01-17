#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/erasable_heap.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<i32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::erasable_heap<i32, std::greater<>> min(a);
    mld::erasable_heap<i32> max(a);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 0) {
            i32 x;
            std::cin >> x;
            min.insert(x), max.insert(x);
        } else if (t == 1) {
            i32 x = min.top();
            min.pop(), max.erase(x);
            std::cout << x << '\n';
        } else {
            i32 x = max.top();
            max.pop(), min.erase(x);
            std::cout << x << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
