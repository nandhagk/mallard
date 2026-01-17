#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "ska_sort.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> p;
    p.reserve(n);
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        p.emplace_back(a, i);
    }
    ska::ska_sort(p.begin(), p.end());

    while (q--) {
        u32 l, r, x;
        std::cin >> l >> r >> x;

        auto a = std::ranges::lower_bound(p, std::make_pair(x, l));
        auto b = std::lower_bound(a, p.end(), std::make_pair(x, r));
        std::cout << b - a << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
