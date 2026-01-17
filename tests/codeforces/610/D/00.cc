#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/offline/rectangle_union.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::offline::rectangle_union<i64, i64> ru;
    ru.reserve(n);

    while (n--) {
        i64 x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);

        ru.add_rectangle(2 * x1 - 1, 2 * y1 - 1, 2 * x2 + 1, 2 * y2 + 1);
    }

    std::cout << ru.area() / 4 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
