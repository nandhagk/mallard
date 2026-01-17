#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/offline/rectangle_union.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::offline::rectangle_union<u32> ru;
    ru.reserve(n);

    while (n--) {
        u32 l, d, r, u;
        std::cin >> l >> d >> r >> u;

        ru.add_rectangle(l, d, r, u);
    }

    std::cout << ru.area() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
