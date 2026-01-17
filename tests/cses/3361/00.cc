#include <bits/stdc++.h>
#include <limits>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n), b(n);
    for (auto &&x : a) std::cin >> x;
    for (auto &&x : b) std::cin >> x;

    f64 lo = 0;
    f64 hi = std::numeric_limits<i32>::max();

    i32 p = -1, q = -1;
    while (hi - lo > 1e-6) {
        f64 x = (hi + lo) / 2;

        i32 c = -1, d = -1;
        f64 u = std::numeric_limits<f64>::lowest();

        f64 y = 0;
        for (u32 i = 0; i < n; ++i) {
            y += a[i] - x;
            if (u < y) u = y, c = i;
        }

        f64 v = std::numeric_limits<f64>::lowest();

        f64 z = 0;
        for (u32 i = 0; i < n; ++i) {
            z += b[i] - x;
            if (v < z) v = z, d = i;
        }

        if (u + v >= 0)
            lo = x, p = c, q = d;
        else
            hi = x;
    }

    std::cout << p + 1 << ' ' << q + 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
