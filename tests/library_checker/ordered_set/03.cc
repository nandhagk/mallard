#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/managers/reusing.h"
#include "lib/ordered_set.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::ordered_set<u32, mld::managers::reusing<>> os(std::views::iota(u32{0}, n) |
                                                       std::views::transform([](auto) {
                                                           u32 a;
                                                           std::cin >> a;
                                                           return a;
                                                       }));

    while (q--) {
        u32 t, x;
        std::cin >> t >> x;

        if (t == 0) {
            os.insert(x);
        } else if (t == 1) {
            os.erase(x);
        } else if (t == 2) {
            if (--x; x < os.size())
                std::cout << os.kth(x) << '\n';
            else
                std::cout << -1 << '\n';
        } else if (t == 3) {
            std::cout << os.rank(++x) << '\n';
        } else if (t == 4) {
            if (os.contains(x))
                std::cout << x << '\n';
            else if (auto y = os.prev(x))
                std::cout << *y << '\n';
            else
                std::cout << -1 << '\n';
        } else //
            if (os.contains(x)) {
                std::cout << x << '\n';
            } else if (auto y = os.next(x)) {
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
