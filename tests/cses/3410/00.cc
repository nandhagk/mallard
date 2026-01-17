#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    i64 a = std::numeric_limits<i64>::lowest();
    i64 b = std::numeric_limits<i64>::max();
    i64 c = std::numeric_limits<i64>::lowest();
    i64 d = std::numeric_limits<i64>::max();
    for (u32 i = 0; i < n; ++i) {
        i32 x, y;
        std::cin >> x >> y;

        a = std::max(a, i64(x) + y);
        b = std::min(b, i64(x) + y);
        c = std::max(c, i64(x) - y);
        d = std::min(d, i64(x) - y);

        std::cout << std::max(a - b, c - d) << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
